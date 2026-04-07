#pragma once

#include "ilogger.hpp"
#include "logging/log_entry.hpp"
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace my_grep::logger {

struct ILoggerBackend;

class Logger final : public ILogger
{
public:
    using LoggerBackendPtr_t = std::unique_ptr<ILoggerBackend>;

    Logger();
    ~Logger();

    Logger(const Logger&) = delete;
    Logger(Logger&&) noexcept = delete;
    Logger& operator=(const Logger&) = delete;
    Logger& operator=(Logger&&) noexcept = delete;

    void logSearchResult(std::filesystem::path filePath
        , size_t line_num, std::string line) override;
    void logMessage(std::string message) override;
    void logError(std::string message) override;
    void addLoggerBackend(LoggerBackendPtr_t backendPtr) override;

private:

    std::queue<LogEntry> m_logQueue{};
    std::thread m_loggingThread{};
    std::mutex m_queueMutex{};
    std::condition_variable m_cv{};
    bool m_done{ false };
    std::vector<LoggerBackendPtr_t> m_loggerBackends{};

private:
    void processQueue();

};

}