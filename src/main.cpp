#include <filesystem>
#include <locale>
#include <codecvt>
#include <iostream>

#include "my_grep.hpp"
#include "logger.hpp"

int main(int argc, char* argv[]) 
{
    namespace fs = std::filesystem;

    if (argc < 3)
    {
        std::wcerr << "Usage: " << argv[0] << " PATTERN DIRECTORY" << std::endl;
        return 1;
    }

    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

    std::wstring pattern{ converter.from_bytes(argv[1]) };
    fs::path path{ argv[2] };

    std::wcout << "Will search for '" << pattern << "' in " << path << std::endl;

    my_grep::logger::Logger loggerInstance{};
    my_grep::MyGrep grep{ loggerInstance };
    grep.search(std::move(path), std::move(pattern));
}
