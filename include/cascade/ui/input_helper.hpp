#ifndef CASCADE_UI_INPUT_HELPER_HPP
#define CASCADE_UI_INPUT_HELPER_HPP

#include <string>
#include <vector>
#include <limits>

namespace cascade
{
    namespace ui
    {
        /**
         *  @brief Console input helper utilities with validation and default values
         *
         * Note: Pressing enter with no value uses default value
         */
        class InputHelper
        {
        public:
            /**
             * @brief Prompt for integer with validation
             * @param prompt Display prompt
             * @param defaultValue Default if user presses Enter
             * @param min Minimum valid value
             * @param max Maximum valid value
             * @return User input or default
             */
            static int promptInt(
                const std::string &prompt,
                int defaultValue,
                int min = std::numeric_limits<int>::min(),
                int max = std::numeric_limits<int>::max());

            /**
             * @brief Prompt for size_t with validation
             * @param prompt Display prompt
             * @param defaultValue Default if user presses Enter
             * @param min Minimum valid value
             * @param max Maximum valid value
             * @return User input or default
             */
            static size_t promptSize(
                const std::string &prompt,
                size_t defaultValue,
                size_t min = 0,
                size_t max = std::numeric_limits<size_t>::max());

            /**
             * @brief Prompt for float with validation
             * @param prompt Display prompt
             * @param defaultValue Default if user presses Enter
             * @param min Minimum valid value
             * @param max Maximum valid value
             * @return User input or default
             */
            static float promptFloat(
                const std::string &prompt,
                float defaultValue,
                float min = std::numeric_limits<float>::lowest(),
                float max = std::numeric_limits<float>::max());

            /**
             * @brief Prompt for bool with validation
             * @param prompt Display prompt
             * @param defaultValue default if user presses Enter
             */
            static bool promptBool(
                const std::string &prompt,
                bool defaultValue);

            /**
             * @brief Prompt for multiple choice
             * @param prompt Display prompt
             * @param options List of options to display
             * @param defaultChoice Default choice (1-indexed)
             * @return Selected option index (0-indexed)
             */
            static size_t promptChoice(
                const std::string &prompt,
                const std::vector<std::string> &options,
                size_t defaultChoice = 1);

            /**
             * @brief Prompt for string
             * @param prompt Display prompt
             * @param defaultValue Default if user presses Enter
             * @return User input or default
             */
            static std::string promptString(
                const std::string &prompt,
                const std::string &defaultValue);

            /**
             * @brief Display section header
             * @param title Section title
             */
            static void printSectionHeader(const std::string &title);

            /**
             * @brief Display subsection header
             * @param title Subsection title
             */
            static void printSubsectionHeader(const std::string &title);

            /**
             * @brief Clear input buffer
             */
            static void clearInputBuffer();

        private:
            static std::string getLine();
            static bool isEmptyOrWhiteSpace(const std::string &str);
        };
    } // namespace ui

} // namespace cascade

#endif