#include "my_grep.hpp"
#include "ilogger.hpp"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

namespace fs = std::filesystem;
using ::testing::_;
using ::testing::Return;

class MockLogger : public my_grep::logger::ILogger
{
public:
    MOCK_METHOD(void, logSearchResult, (std::filesystem::path, size_t, std::wstring), (override));
    MOCK_METHOD(void, logMessage, (std::wstring), (override));
    MOCK_METHOD(void, logError, (std::wstring), (override));
};

class MyGrepTest : public ::testing::Test 
{
protected:

    void SetUp() override
    {
    }

    void TearDown() override 
    {
    }

    MockLogger logger{};
    my_grep::MyGrep grep{ logger };

};


TEST_F(MyGrepTest, TestEmptyFileSearch)
{
    EXPECT_CALL(logger, logSearchResult(_, _, _)).Times(0); 
    grep.search(fs::path{"./test_data/empty.txt"}, L"Test string");
}

TEST_F(MyGrepTest, TestUtf8Search)
{
    EXPECT_CALL(logger, logSearchResult(_, _, _)).Times(7); 
    grep.search(fs::path{ "./test_data/utf8/u8_test.log" }, L"APP");
}

TEST_F(MyGrepTest, TestUtf16Search)
{
    EXPECT_CALL(logger, logSearchResult(_, _, _)).Times(7); 
    grep.search(fs::path{ "./test_data/utf16/u16_test.log" }, L"APP");
}

TEST_F(MyGrepTest, TestUtf8SearchLowerCase)
{
    EXPECT_CALL(logger, logSearchResult(_, _, _)).Times(2); 
    grep.search(fs::path{ "./test_data/utf8/u8_test.log" }, L"app");
}

TEST_F(MyGrepTest, TestUtf16SearchLowerCase)
{
    EXPECT_CALL(logger, logSearchResult(_, _, _)).Times(2); 
    grep.search(fs::path{ "./test_data/utf16/u16_test.log" }, L"app");
}

TEST_F(MyGrepTest, TestFolderSearch)
{
    EXPECT_CALL(logger, logSearchResult(_, _, _)).Times(4); 
    EXPECT_CALL(logger, logMessage(_)).Times(2);

    grep.search(fs::path{ "./test_data/" }, L"app");
}

TEST_F(MyGrepTest, TestFileNotFound)
{
    EXPECT_CALL(logger, logError(_)).Times(1);
    grep.search(fs::path{ "./test_data/definetely_not_there.txt" }, L"app");
}
