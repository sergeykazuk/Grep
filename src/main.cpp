#include "logger.hpp"
#include "paths_tracker.hpp"
#include "logging/consolebackend.hpp"
#include "logging/textfilebackend.hpp"
#include "my_grep.hpp"
#include <filesystem>
#include <iostream>
#include <string>
#include <string_view>

namespace
{

void printUsage(std::string_view executableName)
{
    std::cout << "Usage: " << executableName << " [-c] [-f=fileName] PATTERN DIRECTORY\n"
              << "  -c, --color  Enable colored output\n"
              << "  -f=fileName  Write search results to a file placed next to the searched file\n"
              << "               or inside the searched directory\n"
              << "  -h, --help   Show this help message\n";
}

std::filesystem::path resolveOutputPath(const std::filesystem::path& searchTarget,
                                        const std::filesystem::path& outputFileName)
{
    if (std::filesystem::is_directory(searchTarget))
    {
        return searchTarget / outputFileName;
    }

    return searchTarget.parent_path() / outputFileName;
}

} // namespace

int main(int argc, char* argv[])
{
    namespace fs = std::filesystem;

    bool enableColor{false};
    std::filesystem::path outputFileName{};
    std::string pattern{};
    fs::path path{};

    for (int index = 1; index < argc; ++index)
    {
        const std::string_view argument{argv[index]};

        if (argument == "-h" || argument == "--help")
        {
            printUsage(argv[0]);
            return 0;
        }

        if (argument == "-c" || argument == "--color")
        {
            enableColor = true;
            continue;
        }

        if (argument.starts_with("-f="))
        {
            const auto delimiterPos = argument.find('=');
            outputFileName = fs::path{argument.substr(delimiterPos + 1)};

            if (outputFileName.empty() || outputFileName.has_parent_path())
            {
                std::cerr << "-f expects a file name only, e.g. -f=SearchResults.txt"
                          << std::endl;
                return 1;
            }

            continue;
        }

        if (argument.starts_with('-'))
        {
            std::cerr << "Unknown option: " << argument << std::endl;
            printUsage(argv[0]);
            return 1;
        }

        if (pattern.empty())
        {
            pattern = std::string(argument);
            continue;
        }

        if (path.empty())
        {
            path = fs::path{argument};
            continue;
        }

        std::cerr << "Unexpected argument: " << argument << std::endl;
        printUsage(argv[0]);
        return 1;
    }

    if (pattern.empty() || path.empty())
    {
        printUsage(argv[0]);
        return 1;
    }

    std::cout << "Will search for '" << pattern << "' in " << path << std::endl;

    auto tracker = std::make_shared<my_grep::PathsTracker>();

    namespace ml = my_grep::logger;
    auto loggerInstance = std::make_unique<ml::Logger>(tracker);

    {
        auto consoleBackend = std::make_unique<ml::ConsoleBackend>(enableColor);
        consoleBackend->setSearchPattern(pattern);
        loggerInstance->addLoggerBackend(std::move(consoleBackend));
    }

    if (!outputFileName.empty())
    {
        auto outputPath = resolveOutputPath(path, outputFileName);
        auto fileBackend = std::make_unique<ml::TextFileBackend>(std::move(outputPath));
        fileBackend->setSearchPattern(pattern);
        loggerInstance->addLoggerBackend(std::move(fileBackend));
    }

    my_grep::MyGrep grep{std::move(loggerInstance), std::move(tracker)};
    grep.search(std::move(path), std::move(pattern));
}
