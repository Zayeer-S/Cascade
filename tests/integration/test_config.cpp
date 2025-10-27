#include "cascade/utils/config.hpp"
#include <gtest/gtest.h>

using namespace cascade;

TEST(ConfigTest, DefaultConfigIsValid)
{
    Config config = Config::createDefault();
    auto result = config.validate();

    EXPECT_TRUE(result.isValid) << "Default config should be valid";
}

TEST(ConfigTest, SmallConfigIsValid)
{
    Config config = Config::createSmall();
    auto result = config.validate();

    EXPECT_TRUE(result.isValid) << "Small config should be valid";
}

TEST(ConfigTest, LargeConfigIsValid)
{
    Config config = Config::createLarge();
    auto result = config.validate();

    EXPECT_TRUE(result.isValid) << "Large config should be valid";
}

TEST(ConfigTest, InvalidGridWidth)
{
    Config config = Config::createDefault();
    config.grid.width = 0;

    auto result = config.validate();
    EXPECT_FALSE(result.isValid) << "Config with zero grid width should be invalid";
}

TEST(ConfigTest, InvalidGridHeight)
{
    Config config = Config::createDefault();
    config.grid.height = 0;

    auto result = config.validate();
    EXPECT_FALSE(result.isValid) << "Config with zero grid height should be invalid";
}

TEST(ConfigTest, NegativeGridWidth)
{
    Config config = Config::createDefault();
    config.grid.width = -10;

    auto result = config.validate();
    EXPECT_FALSE(result.isValid) << "Config with negative grid width should be invalid";
}

TEST(ConfigTest, ValidationErrorMessages)
{
    Config config = Config::createDefault();
    config.grid.width = 0;

    auto result = config.validate();
    EXPECT_FALSE(result.isValid);
    EXPECT_FALSE(result.errors.empty()) << "Invalid config should provide error messages";
}