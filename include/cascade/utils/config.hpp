#ifndef CASCADE_UTILS_CONFIG_HPP
#define CASCADE_UTILS_CONFIG_HPP

#include "vector2d.hpp"
#include "validation.hpp"
#include <cstddef>
#include <string>

namespace cascade
{
    struct Config
    {
        struct GridConfig
        {
            size_t width = 50;
            size_t height = 50;
        } grid;

        struct CellConfig
        {
            float initialFuel = 1.0f;               ///< Initial fuel amount in cells
            float initialMoisture = 0.3f;           ///< Initial moisture content
            float smoulderThreshold = 0.3f;         ///< Fuel level to transition to smouldering
            float smoulderBurnRate = 0.03f;         ///< Slower burn rate when smouldering
            float wetEvaporationRate = 0.05f;       ///< Moisture loss per second when wet
            float moistureGainPerWater = 0.1f;      ///< Moisture gained per unit of water
        } cell;

        struct FireConfig
        {
            float baseSpreadRate = 0.3f;            ///< Base probability of spread rate of fire per second
            float ignitionTemp = 300.0f;            ///< Temp required for ignition (celsius)
            float maxIntensity = 1000.0f;           ///< Maximum fire intensity/temp (celsius)
            float burnRate = 0.5f;                  ///< Fuel consumption rate (units/second)
            float heatTransfer = 50.0f;             ///< Heat transfer rate to neighbours (celsius/second)
            float coolingRate = 10.0f;              ///< Cooling rate for burned cells (celsius/second)
            float ambientTemp = 20.0f;              ///< Ambient temperature (celsius)
        } fire;

        struct FireModelConfig
        {
            float windInfluence = 0.5f;             ///< Wind influence multiplier
            float diagonalPenalty = 0.7f;           ///< Spread penalty for diagonal cells
            float intensityMultiplier = 0.5f;       ///< Base intensity multiplier for spread
            float humidityEffect = 0.3f;            ///< Humidity effect on fire spread
        } fireModel;

        struct WindConfig
        {
            Vector2D direction = Vector2D(1.0f, 0.0f); ///< Wind direction (normalised)
            float speed = 5.0f;                        ///< Wind speed (m/s)
            bool enabled = true;                       ///< Whether wind affects speed
        } wind;

        struct SuppressionConfig
        {
            float defaultRange = 3.0f;              ///< Default supression range (cells)
            float defaultFlowRate = 10.0f;          ///< Default water flow rate (unit/s)
            float waterEffectiveness = 100.0f;      ///< Temperature reduction per unit of water
        } suppression;

        struct AgentConfig
        {
            float defaultWaterCapacity = 100.0f;    ///< Default agent water capacity
            float defaultWaterPerCell = 0.5f;       ///< Default water per cell application
            size_t defaultSpawnStep = 0;            ///< Default spawn step (0 = immediate)
            size_t maxAgents = 10;                  ///< Maximum number of agents allowed
        } agent;

        struct SimulationConfig
        {
            float timeStep = 0.1f;              ///< Fixed time step (seconds)
            unsigned int randomSeed = 12345;    ///< Random seed
            bool deterministicMode = false;     ///< Use deterministic random numbers
            size_t maxSteps = 1200;             ///< Maximum simulation steps (0 = unlimited)
        } simulation;

        struct GUIConfig
        {
            int cellSize = 12;                  ///< Size of each cell in pixels
            int windowPaddingX = 200;           ///< Horizontal window padding
            int windowPaddingY = 100;           ///< Vertical window padding
            float targetFPS = 60.0f;            ///< Target frames per second
        } gui;

        /**
         * @brief Create a default config
         * @return Config with default values
         */
        static Config createDefault()
        {
            return Config();
        }

        /**
         * @brief Create a small test config
         * @return Config with small grid
         */
        static Config createSmall()
        {
            Config cfg;
            cfg.grid.width = 20;
            cfg.grid.height = 20;
            return cfg;
        }

        /**
         * @brief Create a large test config
         * @return Config with large grid
         */
        static Config createLarge()
        {
            Config cfg;
            cfg.grid.width = 100;
            cfg.grid.height = 100;
            return cfg;
        }

        /**
         * @brief Load config from file
         * @param path Path to configuration file
         * @return Config loaded from file
         * @note Not implemented for coursework MVP
         */
        static Config loadFromFile(const std::string &path);

        /**
         * @brief Save configuration to file
         * @param path Path to save configuration
         * @return true if successful, false otherwise
         * @note Not implemented for coursework MVP
         */
        bool saveToFile(const std::string &path) const;

        /**
         * @brief Validate configuration values
         * @return true if configuration is valid
         */
        ValidationResult validate() const;
    };
}

#endif