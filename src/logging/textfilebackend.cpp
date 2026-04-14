#include "logging/textfilebackend.hpp"
#include <fstream>
#include <iostream>
#include <chrono>
#include <ctime>

namespace my_grep::logger {

TextFileBackend::TextFileBackend(std::filesystem::path outFile)
    : m_outFile(std::move(outFile))
{}

TextFileBackend::~TextFileBackend()
{
    dumpResultsToFile();
}

void TextFileBackend::write(const LogEntry& e)
{
    m_toLog.push(e);
}

void TextFileBackend::dumpResultsToFile()
{
    std::ofstream out_file(m_outFile, std::ios::trunc);

    if (!out_file.is_open())
    {
        std::cerr << "Cannot create '" << m_outFile << "'\n";
        return;
    }

    using namespace std::chrono;
    auto now_time_t = system_clock::to_time_t(system_clock::now());

    out_file << std::ctime(&now_time_t) << "\n";
    out_file << "MyGrep search results for '" << m_pattern << "':\n";

    while (!m_toLog.empty())
    {
        const auto& front = m_toLog.front();
        out_file << defaultLogFormatter(front, m_pattern) << std::endl;

        m_toLog.pop();
    }

    out_file.close();
}

}