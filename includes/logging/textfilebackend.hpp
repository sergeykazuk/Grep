#pragma once
#include "logging/iloggerbackend.hpp"
#include "logging/log_entry.hpp"
#include <queue>
#include <string>
#include <filesystem>

namespace my_grep::logger {

class TextFileBackend final : public ILoggerBackend
{
public:
    explicit TextFileBackend(std::filesystem::path outFile);
    ~TextFileBackend();

    void write(const LogEntry&) override;

private:
    void dumpResultsToFile();

private:
    std::queue<LogEntry> m_toLog{};
    std::filesystem::path m_outFile;
};

}