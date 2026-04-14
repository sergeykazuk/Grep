#include "paths_tracker.hpp"

namespace my_grep 
{

PathsTracker::PathId_t PathsTracker::createPathId(const fs::path& path)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_pathToIdMap.find(path);
    
    if (it == m_pathToIdMap.end())
    {
        static PathId_t id = 1;
        it = m_pathToIdMap.insert({path, id}).first;
        m_reversePathMap.insert({id, path});
        ++id;
    }

    return it->second;
}

std::string_view PathsTracker::getPathStringForId(PathsTracker::PathId_t pathId) const
{
    std::string_view toReturn;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_reversePathMap.find(pathId);
        if (it != m_reversePathMap.end())
        {
            toReturn = it->second.c_str();
        }
    }

    return toReturn;
}

fs::path PathsTracker::getPathForId(PathsTracker::PathId_t pathId) const
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_reversePathMap.find(pathId);
        if (it != m_reversePathMap.end())
        {
            return it->second;
        }
    }
    return {};
}

}