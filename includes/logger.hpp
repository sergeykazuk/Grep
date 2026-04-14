#pragma once

#include "ilogger.hpp"
#include "logging/log_entry.hpp"
#include "paths_tracker.hpp"
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include <memory>

namespace my_grep::logger
{

struct ILoggerBackend;

class Logger final : public ILogger
{
  public:
    using LoggerBackendPtr_t = std::unique_ptr<ILoggerBackend>;

    Logger(std::shared_ptr<PathsTracker> tracker);
    ~Logger();

    Logger(const Logger&) = delete;
    Logger(Logger&&) noexcept = delete;
    Logger& operator=(const Logger&) = delete;
    Logger& operator=(Logger&&) noexcept = delete;

    void logSearchResult(PathsTracker::PathId_t pathId, size_t line_num, std::string line) override;
    void logMessage(std::string message) override;
    void logError(std::string message) override;
    void addLoggerBackend(LoggerBackendPtr_t backendPtr) override;

  private:
    std::shared_ptr<PathsTracker> m_tracker;
    std::queue<LogEntry> m_logQueue{};
    std::mutex m_queueMutex{};
    std::condition_variable m_cv{};
    bool m_done{false};
    std::vector<LoggerBackendPtr_t> m_loggerBackends{};
    std::thread m_loggingThread{};

  private:
    void processQueue();
};

} // namespace my_grep::logger