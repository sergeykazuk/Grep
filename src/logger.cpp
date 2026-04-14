#include "logger.hpp"
#include "logging/iloggerbackend.hpp"

namespace my_grep::logger
{

Logger::Logger(std::shared_ptr<PathsTracker> tracker) 
    : ILogger()
    , m_tracker(std::move(tracker))
{
    m_loggingThread = std::thread([this]() { processQueue(); });
}

Logger::~Logger()
{
    {
        std::unique_lock<std::mutex> lock(m_queueMutex);
        m_done = true;
    }
    m_cv.notify_one();

    if (m_loggingThread.joinable())
    {
        m_loggingThread.join();
    }
}

void Logger::logSearchResult(PathsTracker::PathId_t pathId, size_t line_num, std::string line)
{
    auto path = m_tracker->getPathStringForId(pathId);
    {
        std::unique_lock<std::mutex> lock(m_queueMutex);
        m_logQueue.push(
            LogEntry{EntryType::eSearchResult, path, line_num, std::move(line)});
    }
    m_cv.notify_one();
}

void Logger::logMessage(std::string message)
{
    {
        std::unique_lock<std::mutex> lock(m_queueMutex);
        m_logQueue.push(LogEntry{EntryType::eMessage, "", std::string::npos, std::move(message)});
    }
    m_cv.notify_one();
}

void Logger::logError(std::string message)
{
    {
        std::unique_lock<std::mutex> lock(m_queueMutex);
        m_logQueue.push(LogEntry{EntryType::eError, "", std::string::npos, std::move(message)});
    }
    m_cv.notify_one();
}

void Logger::addLoggerBackend(LoggerBackendPtr_t backendPtr)
{
    m_loggerBackends.push_back(std::move(backendPtr));
}

void Logger::processQueue()
{
    while (true)
    {
        std::queue<LogEntry> pendingEntries{};
        {
            std::unique_lock<std::mutex> lock(m_queueMutex);
            m_cv.wait(lock, [&]() { return m_done || !m_logQueue.empty(); });

            if (m_done && m_logQueue.empty())
            {
                break;
            }

            pendingEntries.swap(m_logQueue);
        }

        while (!pendingEntries.empty())
        {
            const auto& entry = pendingEntries.front();

            for (auto& backend : m_loggerBackends)
            {
                backend->write(entry);
            }

            pendingEntries.pop();
        }
    }
}

} // namespace my_grep::logger
