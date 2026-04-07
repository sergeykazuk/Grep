#include <filesystem>
#include <iostream>
#include <string>
#include <string_view>
#include "my_grep.hpp"
#include "logger.hpp"
#include "logging/consolebackend.hpp"

namespace {

void printUsage(std::string_view executableName)
{
    std::cout << "Usage: " << executableName << " [-c] PATTERN DIRECTORY\n"
        << "  -c, --color  Enable colored output\n"
        << "  -h, --help   Show this help message\n";
}

}

int main(int argc, char* argv[])
{
    namespace fs = std::filesystem;

    bool enableColor{ false };
    std::string pattern{};
    fs::path path{};

    for (int index = 1; index < argc; ++index)
    {
        const std::string_view argument{ argv[index] };

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
            path = fs::path{ argument };
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

    namespace ml = my_grep::logger;
    auto loggerInstance = std::make_unique<ml::Logger>();
    
    {
        auto consoleBackend = std::make_unique<ml::ConsoleBackend>(enableColor);
        consoleBackend->setSearchPattern(pattern);
        loggerInstance->addLoggerBackend(std::move(consoleBackend));
    }

    my_grep::MyGrep grep{ std::move(loggerInstance) };
    grep.search(std::move(path), std::move(pattern));
}
