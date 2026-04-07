#include "logging/consolebackend.hpp"
#include "logging/log_entry.hpp"
#include <iostream>

namespace my_grep::logger
{

ConsoleBackend::ConsoleBackend(bool enable_color)
{
    if (enable_color)
    {
        m_formatter = colorLogFormatter;
    }
}

void ConsoleBackend::write(const LogEntry& entry)
{
    switch (entry.type)
    {
    case EntryType::eMessage:
    case EntryType::eSearchResult:
        std::cout << m_formatter(entry, m_pattern) << std::endl;
        break;
    case EntryType::eError:
        std::cerr << m_formatter(entry, m_pattern) << std::endl;
        break;
    default:
        break;
    }
}

} // namespace my_grep::logger