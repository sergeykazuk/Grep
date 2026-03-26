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
    virtual void logSearchResult(std::filesystem::path, size_t line_num, std::wstring line) = 0;
    virtual void logMessage(std::wstring message) = 0;
    virtual void logError(std::wstring message) = 0;

};

}
}