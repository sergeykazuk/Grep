#include "my_grep.hpp"
#include "logger.hpp"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

namespace fs = std::filesystem;
using ::testing::_;
using ::testing::Return;

class MockLogger : public my_grep::logger::ILogger
{
public:
    MOCK_METHOD(void, logSearchResult, (std::filesystem::path, size_t
        , std::string), (override));
    MOCK_METHOD(void, logMessage, (std::string), (override));
    MOCK_METHOD(void, logError, (std::string), (override));
    MOCK_METHOD(void, addLoggerBackend, (ILogger::LoggerBackendPtr_t), (override));
};

class MyGrepTest : public ::testing::Test 
{
protected:

    void SetUp() override
    {
        auto loggerPtr = std::make_unique<MockLogger>();
        logger = loggerPtr.get();

        grep = std::make_unique<my_grep::MyGrep>(std::move(loggerPtr));
    }

    void TearDown() override 
    {
        grep.reset();
    }

    MockLogger *logger;
    std::unique_ptr<my_grep::MyGrep> grep;
};


TEST_F(MyGrepTest, TestEmptyFileSearch)
{
    EXPECT_CALL(*logger, logSearchResult(_, _, _)).Times(0); 
    grep->search(fs::path{"./tests/test_data/empty.txt"}, "Test string");
}

TEST_F(MyGrepTest, TestUtf8Search)
{
    EXPECT_CALL(*logger, logSearchResult(_, _, _)).Times(7); 
    grep->search(fs::path{ "./tests/test_data/utf8/u8_test.log" }, "APP");
}

TEST_F(MyGrepTest, TestUtf8SearchLowerCase)
{
    EXPECT_CALL(*logger, logSearchResult(_, _, _)).Times(2); 
    grep->search(fs::path{ "./tests/test_data/utf8/u8_test.log" }, "app");
}

TEST_F(MyGrepTest, TestFolderSearch)
{
    EXPECT_CALL(*logger, logSearchResult(_, _, _)).Times(4); 
    EXPECT_CALL(*logger, logMessage(_)).Times(2);

    grep->search(fs::path{ "./tests/test_data/" }, "app");
}

TEST_F(MyGrepTest, TestFileNotFound)
{
    EXPECT_CALL(*logger, logError(_)).Times(1);
    grep->search(fs::path{ "./tests/test_data/definetely_not_there.txt" }, "app");
}
