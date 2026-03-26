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
    Logger(Logger&&) = default;
    Logger& operator=(const Logger&) = delete;
    Logger& operator=(Logger&&) = default;

    void logSearchResult(std::filesystem::path filePath, size_t line_num, std::wstring line) override;
    void logMessage(std::wstring message) override;
    void logError(std::wstring message) override;

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
        std::wstring str{};
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