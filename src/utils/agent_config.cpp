#include "cascade/utils/agent_config.hpp"
#include "cascade/ui/input_helper.hpp"
#include <iostream>
#include <sstream>

namespace cascade
{
    AgentSpawnConfig AgentSpawnConfig::createInteractive(const Config &config)
    {
        using namespace ui;

        AgentSpawnConfig agentConfig;

        InputHelper::printSectionHeader("Agent Setup");

        bool addAgents = InputHelper::promptBool("Add suppression agents?", false);
        if (!addAgents)
        {
            std::cout << "No agents configured.\n";
            return agentConfig;
        }

        size_t numAgents = InputHelper::promptSize("Number of agents", 1, 0, config.agent.maxAgents);
        if (numAgents < 1)
        {
            std::cout << "Can't make up your mind huh?";
            return agentConfig;
        }

        for (size_t i = 0; i < numAgents; ++i)
        {
            std::cout << "\n--- Agent #" << (i + 1) << " ---\n";

            AgentSpec spec(config);

            size_t typeChoice = InputHelper::promptChoice(
                "Type:",
                {"Aggressive (attacks fire directly)", "Defensive (creates firebreaks)"},
                1);
            spec.type = (typeChoice == 0) ? AgentType::Aggressive : AgentType::Defensive;

            std::string defaultName = "Agent-" + std::to_string(i + 1);
            spec.name = InputHelper::promptString("Name", defaultName);

            spec.waterCapacity = InputHelper::promptFloat(
                "Water capacity",
                config.agent.defaultWaterCapacity,
                1.0f,
                10000.0f);

            spec.waterPerCell = InputHelper::promptFloat(
                "Water per cell",
                config.agent.defaultWaterPerCell,
                0.1f,
                10.0f);

            bool spawnImmediately = InputHelper::promptBool("Spawn immediately?", true);
            if (!spawnImmediately)
            {
                spec.spawnStep = InputHelper::promptSize(
                    "  Spawn at step",
                    0,
                    0,
                    config.simulation.maxSteps);
            }
            else
            {
                spec.spawnStep = 0;
            }

            size_t posChoice = InputHelper::promptChoice(
                "Spawn position:",
                {"Random", "Custom coordinates", "Center of grid"},
                1);

            switch (posChoice)
            {
            case 0:
                spec.spawnMode = SpawnMode::Random;
                break;
            case 1:
                spec.spawnMode = SpawnMode::Custom;
                {
                    float x = InputHelper::promptFloat(
                        "  X position",
                        config.grid.width / 2.0f,
                        0.0f,
                        static_cast<float>(config.grid.width - 1));
                    float y = InputHelper::promptFloat(
                        "  Y position",
                        config.grid.height / 2.0f,
                        0.0f,
                        static_cast<float>(config.grid.height - 1));
                    spec.customPosition = Vector2D(x, y);
                }
                break;
            case 2:
                spec.spawnMode = SpawnMode::Center;
                spec.customPosition = Vector2D(
                    config.grid.width / 2.0f,
                    config.grid.height / 2.0f);
                break;
            }

            agentConfig.addAgent(spec);
        }

        std::cout << "\nAgent configuration complete!\n";
        return agentConfig;
    }

} // namespace cascade