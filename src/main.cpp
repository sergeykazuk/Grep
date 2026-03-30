#include <filesystem>
#include <iostream>
#include <string>
#include "my_grep.hpp"
#include "logger.hpp"

int main(int argc, char* argv[])
{
    namespace fs = std::filesystem;

    if (argc < 3)
    {
        std::cerr << "Usage: " << argv[0] << " PATTERN DIRECTORY" << std::endl;
        return 1;
    }

    std::string pattern(argv[1]);
    fs::path path{ argv[2] };

    std::cout << "Will search for '" << pattern << "' in " << path << std::endl;

    my_grep::logger::Logger loggerInstance{};
    my_grep::MyGrep grep{ loggerInstance };
    grep.search(std::move(path), std::move(pattern));
}
