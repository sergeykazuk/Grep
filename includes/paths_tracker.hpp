#pragma once
#include <unordered_map>
#include <filesystem>
#include <string_view>
#include <mutex>

namespace my_grep 
{
namespace fs = std::filesystem;

class PathsTracker final
{
public:
    using PathId_t = size_t;

    PathId_t createPathId(const fs::path& path);
    std::string_view getPathStringForId(PathId_t pathId) const;
    fs::path getPathForId(PathId_t pathId) const;

private:
    std::unordered_map<fs::path, PathId_t> m_pathToIdMap{};
    std::unordered_map<PathId_t, std::filesystem::path> m_reversePathMap{};
    mutable std::mutex m_mutex{};
};

}