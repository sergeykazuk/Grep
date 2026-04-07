#pragma once
#include "formatters.hpp"
#include <string_view>

namespace my_grep {
namespace logger {

struct LogEntry;
    
struct ILoggerBackend {

    ILoggerBackend() = default;
    virtual ~ILoggerBackend() = default;
    ILoggerBackend(const ILoggerBackend&) = default;
    ILoggerBackend(ILoggerBackend&&) = default;
    ILoggerBackend& operator = (const ILoggerBackend&) = default;
    ILoggerBackend& operator = (ILoggerBackend&&) noexcept = default;

    virtual void write(const LogEntry&) = 0;
    inline void setSearchPattern(std::string_view pattern)
    {
        m_pattern = pattern;
    }
    
    inline void setFormatter(LogFormatter formatter)
    {
        m_formatter = std::move(formatter);
    }

protected:
    std::string_view m_pattern{};
    LogFormatter m_formatter = defaultLogFormatter;
};

}
}