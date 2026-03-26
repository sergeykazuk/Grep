#include "logger.hpp"
#include <iostream>

namespace my_grep::logger
{

Logger::Logger()
    : m_loggingThread([&]() { processQueue(); })
{}

Logger::~Logger()
{
    {
        std::unique_lock<std::mutex> lock(m_queueMutex);
        m_done = true;
        m_cv.notify_one();
    }

    if (m_loggingThread.joinable())
    {
        m_loggingThread.join();
    }
}

void Logger::logSearchResult(std::filesystem::path filePath, size_t line_num, std::wstring line)
{
    std::unique_lock<std::mutex> lock(m_queueMutex);
    m_logQueue.push(LogEntry{ EntryType::eSearchResult, std::move(filePath), line_num, std::move(line)});
    m_cv.notify_one();
}

void Logger::logMessage(std::wstring message)
{
    std::unique_lock<std::mutex> lock(m_queueMutex);
    m_logQueue.push(LogEntry{ EntryType::eMessage, "", std::string::npos, std::move(message)});
    m_cv.notify_one();
}

void Logger::logError(std::wstring message)
{
    std::unique_lock<std::mutex> lock(m_queueMutex);
    m_logQueue.push(LogEntry{ EntryType::eError, "", std::string::npos, std::move(message) });
    m_cv.notify_one();
}

void Logger::processQueue()
{
    while (true)
    {
        std::queue<LogEntry> logQueue{};
        {
            std::unique_lock<std::mutex> lock(m_queueMutex);

            m_cv.wait(lock, [&]() { return  m_done || !m_logQueue.empty(); });

            if (m_done && m_logQueue.empty())
            {
                break;
            }

            logQueue.swap(m_logQueue);
        }

        while (!logQueue.empty())
        {
            const auto& leRef = logQueue.front();
            
            switch (leRef.type)
            {
            case EntryType::eSearchResult:
            {
                std::wcout << leRef.file_path << ": " << leRef.line << " > " << leRef.str << "\n";
                break;
            }
            case EntryType::eMessage:
            {
                std::wcout << leRef.str << "\n";
                break;
            }
            case EntryType::eError:
            {
                std::wcerr << leRef.str << "\n";
                break;
            }
            default:
                std::wcerr << "Unknown log entry type\n";
            }
            
            logQueue.pop();
        }
    }
}

}