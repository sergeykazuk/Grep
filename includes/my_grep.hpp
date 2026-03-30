#pragma once

#include <string>
#include <filesystem>
#include <memory>

namespace my_grep
{

namespace logger
{
struct ILogger;
}

class MyGrep
{
public:
    
    explicit MyGrep(logger::ILogger& logger);
    ~MyGrep();
    MyGrep(const MyGrep&) = delete;
    MyGrep(MyGrep&&) = default;
    MyGrep& operator=(const MyGrep&) = delete;
    MyGrep& operator=(MyGrep&&) = default;


    void search(std::filesystem::path path, std::string pattern) noexcept;

private:
    struct Data;
    std::unique_ptr<Data> m_pimpl{};
};


}
