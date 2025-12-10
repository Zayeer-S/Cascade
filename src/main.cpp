#include "cascade/core/simulation.hpp"
#include "cascade/fire/simple_fire_model.hpp"
#include "cascade/utils/config.hpp"
#include <iostream>
#include <iomanip>

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

void runConsoleSimulation()
{
    std::cout << "=== Cascade Fire Simulation - Console Mode ===\n\n";

    Config config = Config::createSmall();
    std::cout << "Grid size: " << config.grid.width << "x" << config.grid.height << "\n";
    std::cout << "Fire spread rate: " << config.fire.baseSpreadRate << "\n\n";

    Simulation sim(config.grid.width, config.grid.height);

    auto fireModel = std::make_unique<SimpleFireModel>(config.simulation.randomSeed);
    fireModel->setBaseSpreadRate(config.fire.baseSpreadRate);

    sim.initialize(std::move(fireModel));

    size_t centerX = config.grid.width / 2;
    size_t centerY = config.grid.height / 2;
    sim.getGrid().getCell(centerX, centerY).ignite();

    std::cout << "Ignited cell at (" << centerX << ", " << centerY << ")\n";
    std::cout << "Press Enter to start simulation...\n";
    std::cin.get();

    std::cout << "\nRunning simulation...\n";
    printGrid(sim.getGrid());

    const float deltaTime = 0.1f;
    const size_t maxSteps = 50;

    for (size_t step = 0; step < maxSteps && sim.isFireActive(); ++step)
    {
        sim.step(deltaTime);

        if (step % 5 == 0)
        {
            std::cout << "\n--- Step " << step << " (Time: "
                      << std::fixed << std::setprecision(1)
                      << sim.getSimulationTime() << "s) ---\n";

            const auto &stats = sim.getStats();
            std::cout << "Burning: " << stats.burningCells
                      << " | Burned: " << stats.burnedCells
                      << " | Unburned: " << stats.unburnedCells << "\n";

            printGrid(sim.getGrid());
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
}

#ifdef CASCADE_GUI_ENABLED
void runGuiSimulation()
{
    std::cout << "=== Cascade Fire Simulation - GUI Mode ===\n\n";

    Config config = Config::createDefault();

    Simulation sim(config.grid.width, config.grid.height);

    auto fireModel = std::make_unique<SimpleFireModel>(config.simulation.randomSeed);
    fireModel->setBaseSpreadRate(config.fire.baseSpreadRate);

    sim.initialize(std::move(fireModel));

    const int cellSize = 12;
    const int windowWidth = static_cast<int>(config.grid.width * cellSize + 200);
    const int windowHeight = static_cast<int>(config.grid.height * cellSize + 100);

    gui::SDLRenderer renderer("Cascade Fire Simulation", windowWidth, windowHeight);
    gui::GameUI gameUI(renderer, sim);

    std::cout << "GUI initialized. Controls:\n";
    std::cout << "\tLeft Click: Ignite cell\n";
    std::cout << "\tRight Click: Extinguish cell\n";
    std::cout << "\tSPACE: Pause/Resume\n";
    std::cout << "\tR: Reset simulation\n";
    std::cout << "\tESC: Exit\n\n";

    const float targetFPS = 60.0f;
    const float frameTime = 1.0f / targetFPS;

    while (!renderer.shouldClose())
    {
        if (!gameUI.processEvents())
        {
            break;
        }

        gameUI.update(frameTime);

        renderer.clear();
        gameUI.render();
        renderer.present();

        SDL_Delay(static_cast<Uint32>(frameTime * 1000.0f));
    }

    std::cout << "GUI closed.\n";
}
#endif

// https://www.asciiart.eu/text-to-ascii-art
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

    bool useGui = false;

#ifdef CASCADE_GUI_ENABLED
    if (argc > 1 && std::string(argv[1]) == "--gui")
    {
        useGui = true;
    }
    else
    {
        std::cout << "Options:\n";
        std::cout << "\tRun with --gui for interactive visualization\n";
        std::cout << "\tRun without arguments for console mode\n\n";

        std::cout << "Use GUI mode? (y/n): ";
        char choice;
        std::cin >> choice;
        std::cin.ignore(); 
        useGui = (choice == 'y' || choice == 'Y');
    }

    if (useGui)
    {
        runGuiSimulation();
    }
    else
    {
        runConsoleSimulation();
    }
#else
    std::cout << "GUI is not available (SDL2 not found)\n";
    std::cout << "Running in console mode...\n\n";
    runConsoleSimulation();
#endif

    std::cout << "\nThank you for using Cascade!\n";
    return 0;
}