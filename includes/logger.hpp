#pragma once

#include "ilogger.hpp"
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace my_grep
{
namespace logger
{

class Logger : public ILogger
{
public:

    Logger();
    ~Logger();

    Logger(const Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger& operator=(Logger&&) noexcept = delete;

    void logSearchResult(std::filesystem::path filePath, size_t line_num, std::string line) override;
    void logMessage(std::string message) override;
    void logError(std::string message) override;

private:

    enum EntryType
    {
        eSearchResult = 0,
        eError,
        eMessage
    };

    struct LogEntry
    {
        EntryType type{ EntryType::eSearchResult };
        std::filesystem::path file_path{};
        size_t line{ std::string::npos };
        std::string str{};
    };


    std::queue<LogEntry> m_logQueue{};
    std::thread m_loggingThread{};
    std::mutex m_queueMutex{};
    std::condition_variable m_cv{};
    bool m_done{ false };

private:

    void processQueue();
};

}
}