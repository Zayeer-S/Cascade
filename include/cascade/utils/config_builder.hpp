#ifndef CASCADE_UTIL_CONFIG_BUILDER_HPP
#define CASCADE_UTIL_CONFIG_BUILDER_HPP

#include "cascade/utils/config.hpp"

namespace cascade
{
    /**
     * @brief Interactive builder for config
     *
     * Provides section-by-section configuration with ez skip options
     */

    class ConfigBuilder
    {
    public:
        /**
         * @brief Build config interactively through console prompts
         * @param useGui whether GUI mode will be used (affects which sections to show)
         * @return Configured config object
         */
        static Config buildInteractive(bool useGui = false);

        /**
         * @brief Prompt for config preset
         * @return Preset config
         */
        static Config promptForPreset();

    private:
        static void configureGrid(Config &config);
        static void configureFire(Config &config);
        static void configureCell(Config &config);
        static void configureFireModel(Config &config);
        static void configureSimulation(Config &config);
        static void configureGUI(Config &config);

        static void printConfigSummary(const Config &config);
    };

} // namespace cascade

#endif