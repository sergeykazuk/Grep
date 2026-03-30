#pragma once
#include <string>
#include <filesystem>

namespace my_grep
{
namespace logger
{

struct ILogger
{
    virtual ~ILogger() = default;
    virtual void logSearchResult(std::filesystem::path, size_t line_num, std::string line) = 0;
    virtual void logMessage(std::string message) = 0;
    virtual void logError(std::string message) = 0;

};

}
}