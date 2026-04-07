#include "my_grep.hpp"
#include "logger.hpp"
#include <atomic>
#include <condition_variable>
#include <fstream>
#include <future>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

namespace my_grep
{
namespace fs = std::filesystem;

struct MyGrep::Data
{
    Data(std::unique_ptr<logger::ILogger> loggerPtr) : m_logger(std::move(loggerPtr))
    {
        m_threads.reserve(std::thread::hardware_concurrency());
    }

    ~Data()
    {
        if (!m_done)
        {
            std::unique_lock<std::mutex> lock(m_queueMutex);
            m_done = true;
            m_cv.notify_all();
        }

        for (auto& thr : m_threads)
        {
            if (thr.joinable())
            {
                thr.join();
            }
        }
    }

    void worker(std::promise<void>& promise)
    {
        while (!m_done)
        {
            fs::path filePath;
            {
                std::unique_lock<std::mutex> lock(m_queueMutex);
                m_cv.wait(lock, [&]() { return (!m_filesQueue.empty() || m_done); });

                if (m_done && m_filesQueue.empty())
                {
                    break;
                }

                filePath.swap(m_filesQueue.front());
                m_filesQueue.pop();
            }

            searchInFile(filePath);

            // check if all files are processed
            --m_tasksCounter;
            if (m_tasksCounter == 0)
            {
                std::call_once(m_searchDoneFlag, [&]() { promise.set_value(); });
            }
        }
    }

    void setupThreads(std::promise<void>& promise)
    {
        for (size_t i = 0; i < std::thread::hardware_concurrency() - 1; ++i)
        {
            m_threads.emplace_back(&Data::worker, this, std::ref(promise));
        }
    }

    void searchInFile(const fs::path& filePath) noexcept
    {
        std::ifstream file(filePath);

        if (!file)
        {
            m_logger->logError("Error opening file: " + filePath.string());
            return;
        }

        // Only UTF-8 files are supported. Future: add encoding policy.
        std::string line{};
        size_t lineNumber{0};

        while (std::getline(file, line))
        {
            ++lineNumber;
            if (line.find(m_pattern) != std::wstring::npos)
            {
                m_logger->logSearchResult(filePath, lineNumber, line);
            }
        }
    }

    void searchInDirectory()
    {
        for (const auto& entry : fs::recursive_directory_iterator(m_path))
        {
            if (entry.is_regular_file())
            {
                std::unique_lock<std::mutex> lock(m_queueMutex);
                m_filesQueue.push(entry);
                ++m_tasksCounter;
            }
            m_cv.notify_one();
        }

        if (m_tasksCounter == 0)
        {
            m_done = true;
        }
    }

    std::string m_pattern{};
    fs::path m_path{};
    std::vector<std::thread> m_threads{};
    std::condition_variable m_cv{};
    std::mutex m_queueMutex{};
    bool m_done{false};
    std::queue<fs::path> m_filesQueue{};
    std::atomic<size_t> m_tasksCounter{0};
    std::once_flag m_searchDoneFlag{};
    std::unique_ptr<logger::ILogger> m_logger;
};

MyGrep::MyGrep(std::unique_ptr<logger::ILogger> loggerPtr)
    : m_pimpl(std::make_unique<MyGrep::Data>(std::move(loggerPtr)))
{
}

MyGrep::~MyGrep() = default;

void MyGrep::search(std::filesystem::path path, std::string pattern) noexcept
{

    if (path.empty() || pattern.empty())
    {
        m_pimpl->m_logger->logError("Invalid input, please check path or search pattern.");
        return;
    }
    m_pimpl->m_pattern.swap(pattern);
    m_pimpl->m_path.swap(path);

    if (fs::is_regular_file(m_pimpl->m_path))
    {
        // search directly in file
        m_pimpl->searchInFile(m_pimpl->m_path);
    }
    else if (fs::is_directory(m_pimpl->m_path))
    {
        m_pimpl->m_logger->logMessage("Searching in folder " + m_pimpl->m_path.string());

        std::promise<void> completionPromise{};
        std::future<void> completionFuture = completionPromise.get_future();

        m_pimpl->setupThreads(completionPromise);
        m_pimpl->searchInDirectory();

        completionFuture.get();
        m_pimpl->m_logger->logMessage("Search in folder done");
        {
            std::unique_lock<std::mutex> lock(m_pimpl->m_queueMutex);
            m_pimpl->m_done = true;
            m_pimpl->m_cv.notify_all();
        }
    }
    else
    {
        m_pimpl->m_logger->logError("Invalid provided path " + m_pimpl->m_path.string());
    }
}
} // namespace my_grep