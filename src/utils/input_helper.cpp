#include "cascade/ui/input_helper.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>

namespace cascade
{
    namespace ui
    {
        std::string InputHelper::getLine()
        {
            std::string line;
            std::getline(std::cin, line);
            return line;
        }

        bool InputHelper::isEmptyOrWhiteSpace(const std::string &str)
        {
            return str.empty() || std::all_of(str.begin(), str.end(), [](unsigned char c)
                                              { return std::isspace(c); });
        }

        void InputHelper::clearInputBuffer()
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        int InputHelper::promptInt(const std::string &prompt, int defaultValue, int min, int max)
        {
            std::cout << prompt << " [" << defaultValue << "]: ";
            while (true)
            {
                std::string input = getLine();

                if (isEmptyOrWhiteSpace(input))
                {
                    return defaultValue;
                }

                try
                {
                    int value = std::stoi(input);
                    if (value < min || value > max)
                    {
                        std::cout << "\tError: Value must be between " << min << "and" << max << "";
                        continue;
                    }
                    return value;
                }
                catch (...)
                {
                    std::cout << "\tError: Please enter a valid integer";
                }
            }
        }

        size_t InputHelper::promptSize(const std::string &prompt, size_t defaultValue, size_t min, size_t max)
        {
            std::cout << prompt << " [" << defaultValue << "]: ";
            while (true)
            {
                std::string input = getLine();

                if (isEmptyOrWhiteSpace(input))
                    return defaultValue;

                try
                {
                    long long value = std::stoll(input);
                    if (value < 0)
                    {
                        std::cout << "\tError: Value cannot be negative";
                        continue;
                    }
                    size_t sizeValue = static_cast<size_t>(value);
                    if (sizeValue < min || sizeValue > max)
                    {
                        std::cout << "\tError: Value must be between " << min << " and " << max;
                        continue;
                    }
                    return sizeValue;
                }
                catch (...)
                {
                    std::cout << "\tError: Please enter a valid number";
                }
            }
        }

        float InputHelper::promptFloat(const std::string &prompt, float defaultValue, float min, float max)
        {
            std::cout << prompt << " [" << defaultValue << "]: ";
            while (true)
            {
                std::string input = getLine();

                if (isEmptyOrWhiteSpace(input))
                {
                    return defaultValue;
                }

                try
                {
                    float value = std::stof(input);
                    if (value < min || value > max)
                    {
                        std::cout << "\tError: Value must be between " << min << " and " << max;
                        continue;
                    }
                    return value;
                }
                catch (...)
                {
                    std::cout << "\tError: Please enter a valid number\n";
                }
            }
        }

        bool InputHelper::promptBool(const std::string &prompt, bool defaultValue)
        {
            std::string defaultStr = defaultValue ? "y" : "n";

            std::cout << prompt << " (y/n) [" << defaultStr << "]: ";
            while (true)
            {
                std::string input = getLine();
                if (isEmptyOrWhiteSpace(input))
                    return defaultValue;

                std::transform(input.begin(), input.end(), input.begin(), [](unsigned char c)
                               { return std::tolower(c); });

                if (input == "y" || input == "ye" || input == "yes")
                    return true;

                else if (input == "n" || input == "no")
                    return false;

                else
                    std::cout << "\tError: Please enter 'y' or 'n'";
            }
        }

        size_t InputHelper::promptChoice(const std::string &prompt, const std::vector<std::string> &options, size_t defaultChoice)
        {
            {
                std::cout << prompt;
                for (size_t i = 0; i < options.size(); ++i)
                    std::cout << "  " << (i + 1) << ". " << options[i];

                std::cout << "Choose [" << defaultChoice << "]: ";
                while (true)
                {
                    std::string input = getLine();

                    if (isEmptyOrWhiteSpace(input))
                    {
                        if (defaultChoice >= 1 && defaultChoice <= options.size())
                            return defaultChoice - 1;
                    }

                    try
                    {
                        int choice = std::stoi(input);
                        if (choice < 1 || choice > static_cast<int>(options.size()))
                        {
                            std::cout << "\tError: Please choose between 1 and " << options.size();
                            continue;
                        }
                        return static_cast<size_t>(choice - 1);
                    }
                    catch (...)
                    {
                        std::cout << "\tError: Please enter a valid number";
                    }
                }
            }
        }

        std::string InputHelper::promptString(const std::string &prompt, const std::string &defaultValue)
        {
            std::cout << prompt;
            if (!defaultValue.empty())
                std::cout << " [" << defaultValue << "]";
            std::cout << ": ";

            std::string input = getLine();
            if (isEmptyOrWhiteSpace(input))
                return defaultValue;
            return input;
        }

        void InputHelper::printSectionHeader(const std::string &title)
        {
            std::cout << "\n=== " << title << " ===\n\n";
        }

        void InputHelper::printSubsectionHeader(const std::string &title)
        {
            std::cout << "\n[" << title << "]\n";
        }
    } // namespace ui
} // namespace cascade
