#pragma once

#include <filesystem>
#include <memory>
#include <string>

namespace my_grep
{

namespace logger
{
class ILogger;
}

class MyGrep final
{
  public:
    explicit MyGrep(std::unique_ptr<logger::ILogger> loggerPtr);
    ~MyGrep();
    MyGrep(const MyGrep&) = delete;
    MyGrep(MyGrep&&) = delete;
    MyGrep& operator=(const MyGrep&) = delete;
    MyGrep& operator=(MyGrep&&) = delete;

    void search(std::filesystem::path path, std::string pattern) noexcept;

  private:
    struct Data;
    std::unique_ptr<Data> m_pimpl{};
};

} // namespace my_grep
