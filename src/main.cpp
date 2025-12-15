#include "cascade/core/simulation.hpp"
#include "cascade/fire/simple_fire_model.hpp"
#include "cascade/utils/config.hpp"
#include "cascade/utils/config_builder.hpp"
#include "cascade/utils/agent_config.hpp"
#include "cascade/suppression/agent_factory.hpp"
#include "cascade/ui/input_helper.hpp"
#include <iostream>
#include <iomanip>
#include <vector>
#include <memory>

#ifdef CASCADE_GUI_ENABLED
#include "cascade/gui/sdl_renderer.hpp"
#include "cascade/gui/game_ui.hpp"
#endif

using namespace cascade;

void printGrid(const Grid &grid)
{
    std::cout << "\n";
    for (size_t y = 0; y < grid.getHeight(); ++y)
    {
        for (size_t x = 0; x < grid.getWidth(); ++x)
        {
            const Cell &cell = grid.getCell(x, y);
            std::cout << cellStateToChar(cell.getState());
        }
        std::cout << "\n";
    }
}

void printAgentSummary(const AgentSpawnConfig &agentConfig)
{
    if (!agentConfig.hasAgents())
    {
        std::cout << "Agents: None\n";
        return;
    }

    std::cout << "Agents: " << agentConfig.getAgentCount() << "\n";
    for (const auto &spec : agentConfig.agents)
    {
        std::string typeStr = (spec.type == AgentSpawnConfig::AgentType::Aggressive)
                                  ? "Aggressive"
                                  : "Defensive";
        std::cout << "  - " << spec.name << " (" << typeStr << ", "
                  << spec.waterCapacity << "L";

        if (spec.spawnStep > 0)
        {
            std::cout << ", spawns at step " << spec.spawnStep;
        }

        std::cout << ")\n";
    }
}

void runConsoleSimulation(const Config &config, const AgentSpawnConfig &agentConfig)
{
    ui::InputHelper::printSectionHeader("Starting Simulation - Console Mode");

    std::cout << "Grid: " << config.grid.width << "x" << config.grid.height << "\n";
    std::cout << "Max steps: " << config.simulation.maxSteps << " ("
              << std::fixed << std::setprecision(1)
              << (config.simulation.maxSteps * config.simulation.timeStep) << " seconds)\n";
    printAgentSummary(agentConfig);
    std::cout << "\n";

    Simulation sim(config);

    auto fireModel = std::make_unique<SimpleFireModel>(config);
    sim.initialize(std::move(fireModel));

    // Create and add agents that spawn immediately (step 0)
    std::vector<std::unique_ptr<SuppressionAgent>> activeAgents;
    std::vector<std::pair<std::unique_ptr<SuppressionAgent>, size_t>> pendingAgents;

    for (const auto &spec : agentConfig.agents)
    {
        auto agent = AgentFactory::createAgent(spec);

        if (spec.spawnStep == 0)
        {
            AgentFactory::setAgentPosition(agent.get(), spec, sim.getGrid());
            sim.addObserver(agent.get());
            activeAgents.push_back(std::move(agent));
        }
        else
        {
            pendingAgents.emplace_back(std::move(agent), spec.spawnStep);
        }
    }

    // Ignite center cell
    size_t centerX = config.grid.width / 2;
    size_t centerY = config.grid.height / 2;
    std::cout << "Igniting cell at (" << centerX << ", " << centerY << ")\n";
    sim.getGrid().getCell(centerX, centerY).ignite();
    sim.refresh();

    std::cout << "\nPress Enter to start simulation...\n";
    std::cin.get();

    std::cout << "\nRunning simulation...\n";

    const float printInterval = 5.0f;
    const size_t printEveryNSteps = static_cast<size_t>(printInterval / config.simulation.timeStep);

    // Print initial state (step 0, time 0.0s)
    printGrid(sim.getGrid());

    for (size_t step = 0; step < config.simulation.maxSteps; ++step)
    {
        for (auto it = pendingAgents.begin(); it != pendingAgents.end();)
        {
            if (step >= it->second)
            {
                std::cout << "\n>>> Agent spawning at step " << step << "!\n";

                for (const auto &spec : agentConfig.agents)
                {
                    if (spec.name == it->first->getName())
                    {
                        AgentFactory::setAgentPosition(it->first.get(), spec, sim.getGrid());
                        break;
                    }
                }

                sim.addObserver(it->first.get());
                activeAgents.push_back(std::move(it->first));
                it = pendingAgents.erase(it);
            }
            else
            {
                ++it;
            }
        }

        sim.step();

        // Print every N steps (prints at steps printEveryNSteps, 2*printEveryNSteps, etc.)
        // After stepping, so time is accurate
        if ((step + 1) % printEveryNSteps == 0)
        {
            std::cout << "\n--- Step " << (step + 1) << " (Time: "
                      << std::fixed << std::setprecision(1)
                      << sim.getSimulationTime() << "s) ---\n";

            const auto &stats = sim.getStats();
            std::cout << "Burning: " << stats.burningCells
                      << " | Burned: " << stats.burnedCells
                      << " | Unburned: " << stats.unburnedCells;

            // Print agent status
            if (!activeAgents.empty())
            {
                std::cout << " | Agents: " << activeAgents.size() << " active";
            }
            std::cout << "\n";

            printGrid(sim.getGrid());

            // Print agent water levels
            for (const auto &agent : activeAgents)
            {
                float waterPercent = (agent->getWaterRemaining() / agent->getWaterCapacity()) * 100.0f;
                std::cout << "  " << agent->getName() << ": "
                          << static_cast<int>(waterPercent) << "% water remaining\n";
            }
        }

        if (!sim.isFireActive() && pendingAgents.empty())
        {
            std::cout << "\nFire extinguished at step " << step << "!\n";
            break;
        }
    }

    std::cout << "\n=== Simulation Complete ===\n";
    const auto &stats = sim.getStats();
    std::cout << "Total steps: " << stats.stepCount << "\n";
    std::cout << "Simulation time: " << std::fixed << std::setprecision(2)
              << stats.simulationTime << "s\n";
    std::cout << "Cells burned: " << stats.burnedCells << " / " << stats.getTotalCells()
              << " (" << std::fixed << std::setprecision(1) << stats.percentBurned << "%)\n";
    std::cout << "Total ignitions: " << stats.totalCellsIgnited << "\n";

    if (!activeAgents.empty())
    {
        std::cout << "\nAgent Statistics:\n";
        for (const auto &agent : activeAgents)
        {
            std::cout << "  " << agent->getName() << ": "
                      << agent->getCellsSuppressed() << " cells suppressed\n";
        }
    }
}

#ifdef CASCADE_GUI_ENABLED
void runGuiSimulation(const Config &config, const AgentSpawnConfig &agentConfig)
{
    ui::InputHelper::printSectionHeader("Starting Simulation - GUI Mode");

    const int windowWidth = static_cast<int>(config.grid.width * config.gui.cellSize + config.gui.windowPaddingX);
    const int windowHeight = static_cast<int>(config.grid.height * config.gui.cellSize + config.gui.windowPaddingY);

    Simulation sim(config);
    auto fireModel = std::make_unique<SimpleFireModel>(config);
    sim.initialize(std::move(fireModel));

    std::vector<std::unique_ptr<SuppressionAgent>> agents;
    for (const auto &spec : agentConfig.agents)
    {
        if (spec.spawnStep == 0) // GUI only supports immediate spawn for MVP
        {
            auto agent = AgentFactory::createAgent(spec);
            AgentFactory::setAgentPosition(agent.get(), spec, sim.getGrid());
            sim.addObserver(agent.get());
            agents.push_back(std::move(agent));
        }
    }

    gui::SDLRenderer renderer("Cascade Fire Simulation", windowWidth, windowHeight);
    gui::GameUI gameUI(renderer, sim);

    // Add agents to UI
    for (const auto &agent : agents)
    {
        gameUI.addAgent(agent.get());
    }

    std::cout << "GUI initialized. Controls:\n";
    std::cout << "\tLeft Click: Ignite cell\n";
    std::cout << "\tRight Click: Extinguish cell\n";
    std::cout << "\tSPACE: Pause/Resume\n";
    std::cout << "\tR: Reset simulation\n";
    std::cout << "\tESC: Exit\n\n";

    const float frameTime = 1.0f / config.gui.targetFPS;

    while (!renderer.shouldClose())
    {
        if (!gameUI.processEvents())
        {
            break;
        }

        gameUI.update(config.simulation.timeStep);

        renderer.clear();
        gameUI.render();
        renderer.present();

        SDL_Delay(static_cast<Uint32>(frameTime * 1000.0f));
    }

    std::cout << "GUI closed.\n";
}
#endif

int main(int argc, char *argv[])
{
    std::cout << R"(
   ____                        _      
  / ___|__ _ ___  ___ __ _  __| | ___ 
 | |   / _` / __|/ __/ _` |/ _` |/ _ \
 | |__| (_| \__ \ (_| (_| | (_| |  __/
  \____\__,_|___/\___\__,_|\__,_|\___|
                                       
  Fire Spread Simulation Engine
  Version 0.1.0
)" << "\n";

    ui::InputHelper::printSectionHeader("Setup Wizard");

    // Step 1: Choose simulation mode
    bool useGui = false;

#ifdef CASCADE_GUI_ENABLED
    if (argc > 1 && std::string(argv[1]) == "--gui")
    {
        useGui = true;
    }
    else
    {
        size_t modeChoice = ui::InputHelper::promptChoice(
            "Simulation Mode:",
            {"Console Mode", "GUI Mode (interactive)"},
            1);
        useGui = (modeChoice == 1);
    }
#else
    std::cout << "GUI is not available (SDL2 not found)\n";
    std::cout << "Using Console Mode\n\n";
#endif

    // Step 2: Configure simulation
    Config config = ConfigBuilder::promptForPreset();

    // Step 3: Configure agents
    AgentSpawnConfig agentConfig = AgentSpawnConfig::createInteractive(config);

    // Step 4: Show summary
    ui::InputHelper::printSectionHeader("Ready to Start");
    std::cout << "Grid: " << config.grid.width << "x" << config.grid.height << "\n";
    std::cout << "Fire spread rate: " << config.fire.baseSpreadRate << "\n";
    std::cout << "Max steps: " << config.simulation.maxSteps
              << " (" << std::fixed << std::setprecision(1)
              << (config.simulation.maxSteps * config.simulation.timeStep) << " seconds)\n";
    printAgentSummary(agentConfig);
    std::cout << "\n";

    // Step 5: Run simulation
#ifdef CASCADE_GUI_ENABLED
    if (useGui)
    {
        runGuiSimulation(config, agentConfig);
    }
    else
    {
        runConsoleSimulation(config, agentConfig);
    }
#else
    runConsoleSimulation(config, agentConfig);
#endif

    std::cout << "\nThank you for using Cascade!\n";
    std::cout << "\nPress Enter to exit...";
    ui::InputHelper::clearInputBuffer();
    std::cin.get();
    return 0;
}