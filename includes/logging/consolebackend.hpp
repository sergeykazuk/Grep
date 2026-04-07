#pragma once

#include "iloggerbackend.hpp"

namespace my_grep
{
namespace logger
{

class ConsoleBackend final : public ILoggerBackend
{
  public:
    explicit ConsoleBackend(bool enable_color = false);
    void write(const LogEntry&) override;
};

} // namespace logger
} // namespace my_grep