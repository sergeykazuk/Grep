#include "logging/formatters.hpp"
#include "logging/log_entry.hpp"
#include <format>

namespace my_grep::logger {

std::string defaultLogFormatter(const LogEntry& entry, std::string_view)
{
    switch (entry.type)
    {
    case EntryType::eSearchResult:
        return std::format("{}: {} > {}", entry.file_path.string()
            , entry.line, entry.str);
    case EntryType::eMessage:
    case EntryType::eError:
        return entry.str;
    default:
        return "Unknown log entry type";
    }
}

std::string colorLogFormatter(const LogEntry& entry, std::string_view pattern)
{
    auto result = defaultLogFormatter(std::move(entry), pattern);

    if (!pattern.empty())
    {
        static constexpr std::string_view color = "\033[91m";
        static constexpr std::string_view reset = "\033[0m";

        size_t pos = 0;
        while ((pos = result.find(pattern, pos)) != std::string::npos)
        {
            result.insert(pos, color);
            pos += color.length() + pattern.length();
            result.insert(pos, reset);
            pos += reset.length();
        }
    }

    return result;
}

}