#pragma once

#include <functional>
#include <string>
#include <string_view>

namespace my_grep
{
namespace logger
{

struct LogEntry;

using LogFormatter = std::function<std::string(const LogEntry&, std::string_view pattern)>;

std::string defaultLogFormatter(const LogEntry& entry, std::string_view pattern);
std::string colorLogFormatter(const LogEntry& entry, std::string_view pattern);

} // namespace logger
} // namespace my_grep
