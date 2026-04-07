#pragma once

#include <cstdint>
#include <string>
#include <filesystem>

namespace my_grep {
namespace logger {

enum class EntryType: uint8_t
{
    eSearchResult = 0,
    eError,
    eMessage
};

struct LogEntry
{
    EntryType type{ EntryType::eSearchResult };
    std::filesystem::path file_path{};
    size_t line{ std::string::npos };
    std::string str{};
};

}
}