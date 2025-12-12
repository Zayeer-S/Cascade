#include "cascade/utils/config_builder.hpp"
#include "cascade/ui/input_helper.hpp"
#include <iostream>
#include <iomanip>

namespace cascade
{
    using namespace ui;

    Config ConfigBuilder::promptForPreset()
    {
        InputHelper::printSectionHeader("Configuration");

        size_t choice = InputHelper::promptChoice(
            "Choose preset:",
            {"Small (20x20)",
             "Default (50x50)",
             "Large (100x100)",
             "Custom"},
            2);

        switch (choice)
        {
        case 0:
            return Config::createSmall();
        case 1:
            return Config::createDefault();
        case 2:
            return Config::createLarge();
        case 3:
            return buildInteractive(false);
        default:
            return Config::createDefault();
        }
    }

    Config ConfigBuilder::buildInteractive(bool useGui)
    {
        Config config = Config::createDefault();

        InputHelper::printSectionHeader("Custom Configuration");

        std::cout << "Configure each section (press Enter to skip and use defaults)\n";

        configureGrid(config);

        configureFire(config);

        configureCell(config);

        configureFireModel(config);

        configureSimulation(config);

        if (useGui)
            configureGUI(config);

        std::cout << "\n=== Configuration Complete ===";
        printConfigSummary(config);

        return config;
    }

    void ConfigBuilder::configureGrid(Config &config)
    {
        InputHelper::printSubsectionHeader("Grid");

        bool customize = InputHelper::promptBool("Customise grid?", false);
        if (!customize)
            return;

        config.grid.width = InputHelper::promptSize("Width", config.grid.width, 10, 200);
        config.grid.height = InputHelper::promptSize("Height", config.grid.height, 10, 200);
    }

    void ConfigBuilder::configureFire(Config &config)
    {
        InputHelper::printSubsectionHeader("Fire");

        bool customize = InputHelper::promptBool("Customize fire behaviour?", false);
        if (!customize)
            return;

        config.fire.baseSpreadRate = InputHelper::promptFloat(
            "Base spread rate", config.fire.baseSpreadRate, 0.0f, 1.0f);

        config.fire.burnRate = InputHelper::promptFloat(
            "Burn rate", config.fire.burnRate, 0.01f, 2.0f);

        config.fire.ignitionTemp = InputHelper::promptFloat(
            "Ignition temperature", config.fire.ignitionTemp, 100.0f, 500.0f);

        config.fire.maxIntensity = InputHelper::promptFloat(
            "Max intensity", config.fire.maxIntensity, 500.0f, 2000.0f);

        config.fire.heatTransfer = InputHelper::promptFloat(
            "Heat transfer rate", config.fire.heatTransfer, 10.0f, 200.0f);

        config.fire.coolingRate = InputHelper::promptFloat(
            "Cooling rate", config.fire.coolingRate, 1.0f, 50.0f);
    }

    void ConfigBuilder::configureCell(Config &config)
    {
        InputHelper::printSubsectionHeader("Cell");

        bool customize = InputHelper::promptBool("Customize cell properties?", false);
        if (!customize)
            return;

        config.cell.initialFuel = InputHelper::promptFloat(
            "Initial fuel", config.cell.initialFuel, 0.1f, 5.0f);

        config.cell.initialMoisture = InputHelper::promptFloat(
            "Initial moisture", config.cell.initialMoisture, 0.0f, 1.0f);

        config.cell.smoulderThreshold = InputHelper::promptFloat(
            "Smoulder threshold", config.cell.smoulderThreshold, 0.1f, 0.9f);

        config.cell.smoulderBurnRate = InputHelper::promptFloat(
            "Smoulder burn rate", config.cell.smoulderBurnRate, 0.01f, 0.2f);
    }

    void ConfigBuilder::configureFireModel(Config &config)
    {
        InputHelper::printSubsectionHeader("Fire Model");

        bool customize = InputHelper::promptBool("Customize fire spread model?", false);
        if (!customize)
            return;

        config.fireModel.windInfluence = InputHelper::promptFloat(
            "Wind influence", config.fireModel.windInfluence, 0.0f, 2.0f);

        config.fireModel.diagonalPenalty = InputHelper::promptFloat(
            "Diagonal penalty", config.fireModel.diagonalPenalty, 0.1f, 1.0f);

        config.fireModel.intensityMultiplier = InputHelper::promptFloat(
            "Intensity multiplier", config.fireModel.intensityMultiplier, 0.1f, 2.0f);

        config.fireModel.humidityEffect = InputHelper::promptFloat(
            "Humidity effect", config.fireModel.humidityEffect, 0.0f, 1.0f);
    }

    void ConfigBuilder::configureSimulation(Config &config)
    {
        InputHelper::printSubsectionHeader("Simulation");

        bool customize = InputHelper::promptBool("Customize simulation parameters?", false);
        if (!customize)
            return;

        config.simulation.maxSteps = InputHelper::promptSize(
            "Max steps", config.simulation.maxSteps, 10, 100000);

        config.simulation.timeStep = InputHelper::promptFloat(
            "Time step (seconds)", config.simulation.timeStep, 0.01f, 1.0f);

        config.simulation.randomSeed = static_cast<unsigned int>(InputHelper::promptInt(
            "Random seed (0=random)", config.simulation.randomSeed, 0, 999999));
    }

    void ConfigBuilder::configureGUI(Config &config)
    {
        InputHelper::printSubsectionHeader("GUI");

        bool customize = InputHelper::promptBool("Customize GUI settings?", false);
        if (!customize)
            return;

        config.gui.cellSize = InputHelper::promptInt(
            "Cell size (pixels)", config.gui.cellSize, 5, 50);

        config.gui.targetFPS = InputHelper::promptFloat(
            "Target FPS", config.gui.targetFPS, 10.0f, 144.0f);
    }

    void ConfigBuilder::printConfigSummary(const Config &config)
    {
        InputHelper::printSectionHeader("Configuration Summary");

        std::cout << "Grid: " << config.grid.width << "x" << config.grid.height << "\n";
        std::cout << "Fire spread rate: " << config.fire.baseSpreadRate << "\n";
        std::cout << "Burn rate: " << config.fire.burnRate << "\n";
        std::cout << "Max steps: " << config.simulation.maxSteps
                  << " (" << std::fixed << std::setprecision(1)
                  << (config.simulation.maxSteps * config.simulation.timeStep)
                  << " seconds)\n";
        std::cout << "Time step: " << config.simulation.timeStep << "s\n";
    }
} // namespace cascade
