#include "cascade/utils/config.hpp";
#include <iostream>

using namespace cascade;

struct ConfigTestCase
{
    std::string name;
    Config config;
    bool expectedValid;
};

void testConfig()
{
    std::cout << "=== Testing Config ===";

    int passCount = 0;
    int failCount = 0;

    Config invalidConfig;
    invalidConfig.grid.width = 0;

    std::vector<ConfigTestCase> tests = {
        {"default", Config::createDefault(), true},
        {"small", Config::createSmall(), true},
        {"large", Config::createLarge(), true},
        {"invalid", invalidConfig, false}};

    for (const auto test : tests)
    {
        auto result = test.config.validate();

        bool pass = (result.isValid == test.expectedValid);

        if (pass)
        {
            passCount++;
        }
        else
        {
            failCount++;
        }

        std::cout << "Validating" << test.name << " config: " << (pass ? "\tPASS" : "\tFAIL");
    }

    std::cout << "=== Finished Testing Config (" << passCount << "/" << passCount + failCount << ")";
}