#pragma once

#include "logging/iloggerbackend.hpp"
#include <string>
#include <filesystem>
#include <memory>

namespace my_grep {
namespace logger {

class ILogger
{
public:
    using LoggerBackendPtr_t = std::unique_ptr<ILoggerBackend>;
    
    ILogger() = default;
    virtual ~ILogger() = default;
    ILogger(const ILogger&) = default;
    ILogger(ILogger&&) noexcept = default;
    ILogger& operator=(const ILogger&) = default;
    ILogger& operator=(ILogger&&) noexcept = default;

    virtual void logSearchResult(std::filesystem::path filePath
        , size_t line_num, std::string line) = 0;
    virtual void logMessage(std::string message) = 0;
    virtual void logError(std::string message) = 0;
    virtual void addLoggerBackend(LoggerBackendPtr_t backendPtr) = 0;
};

}
}