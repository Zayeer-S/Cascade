#include "cascade/gui/game_ui.hpp"
#include <iostream>

namespace cascade
{
    namespace gui
    {

        GameUI::GameUI(SDLRenderer &renderer, Simulation &simulation)
            : renderer_(renderer), simulation_(simulation), agents_(), paused_(false), cellSize_(15), gridOffsetX_(20), gridOffsetY_(20), showHelp_(true), mouseGridX_(-1), mouseGridY_(-1)
        {
            int availWidth = renderer_.getWidth() - 40;
            int availHeight = renderer_.getHeight() - 200;

            int cellsX = static_cast<int>(simulation_.getGrid().getWidth());
            int cellsY = static_cast<int>(simulation_.getGrid().getHeight());

            int maxCellWidth = availWidth / cellsX;
            int maxCellHeight = availHeight / cellsY;

            cellSize_ = std::min(maxCellWidth, maxCellHeight);
            cellSize_ = std::max(cellSize_, 5);
            cellSize_ = std::min(cellSize_, 30);
        }

        bool GameUI::processEvents()
        {
            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                {
                    return false;
                }

                if (event.type == SDL_KEYDOWN)
                {
                    handleKeyPress(event.key.keysym.sym);
                }

                if (event.type == SDL_MOUSEBUTTONDOWN)
                {
                    handleMouseClick(event.button.x, event.button.y, event.button.button);
                }

                if (event.type == SDL_MOUSEMOTION)
                {
                    size_t gx, gy;
                    if (screenToGrid(event.motion.x, event.motion.y, gx, gy))
                    {
                        mouseGridX_ = static_cast<int>(gx);
                        mouseGridY_ = static_cast<int>(gy);
                    }
                    else
                    {
                        mouseGridX_ = -1;
                        mouseGridY_ = -1;
                    }
                }
            }

            return true;
        }

        void GameUI::update(float deltaTime)
        {
            if (!paused_ && simulation_.isFireActive())
            {
                simulation_.step(deltaTime);
            }
        }

        void GameUI::render()
        {
            renderer_.renderGrid(simulation_.getGrid(), cellSize_, gridOffsetX_, gridOffsetY_);

            if (mouseGridX_ >= 0 && mouseGridY_ >= 0)
            {
                int px = gridOffsetX_ + mouseGridX_ * cellSize_;
                int py = gridOffsetY_ + mouseGridY_ * cellSize_;
                renderer_.renderRectOutline(px, py, cellSize_, cellSize_, Colour(255, 255, 255, 200));
            }

            renderStats();
            renderAgents();

            if (showHelp_)
            {
                renderHelp();
            }
        }

        void GameUI::addAgent(SuppressionAgent *agent)
        {
            if (agent)
            {
                agents_.push_back(agent);
            }
        }

        void GameUI::handleMouseClick(int x, int y, int button)
        {
            size_t gx, gy;
            if (!screenToGrid(x, y, gx, gy))
            {
                return;
            }

            Cell &cell = simulation_.getGrid().getCell(gx, gy);

            if (button == SDL_BUTTON_LEFT)
            {
                if (cell.canIgnite())
                {
                    cell.ignite();
                    std::cout << "Ignited cell (" << gx << ", " << gy << ")\n";
                }
            }
            else if (button == SDL_BUTTON_RIGHT)
            {
                if (cell.isBurning() || cell.getState() == CellState::Unburned)
                {
                    cell.extinguish(1.0f);
                    std::cout << "Applied water to cell (" << gx << ", " << gy << ")\n";
                }
            }
        }

        void GameUI::handleKeyPress(SDL_Keycode key)
        {
            switch (key)
            {
            case SDLK_SPACE:
                paused_ = !paused_;
                std::cout << (paused_ ? "PAUSED" : "RESUMED") << "\n";
                break;

            case SDLK_r:
                simulation_.reset();
                std::cout << "Simulation RESET\n";
                break;

            case SDLK_h:
                showHelp_ = !showHelp_;
                break;

            case SDLK_ESCAPE:
                renderer_.setShouldClose(true);
                break;

            case SDLK_PLUS:
            case SDLK_EQUALS:
                std::cout << "Speed up\n";
                break;

            case SDLK_MINUS:
                std::cout << "Slow down\n";
                break;

            default:
                break;
            }
        }

        bool GameUI::screenToGrid(int screenX, int screenY, size_t &gridX, size_t &gridY)
        {
            int localX = screenX - gridOffsetX_;
            int localY = screenY - gridOffsetY_;

            if (localX < 0 || localY < 0)
            {
                return false;
            }

            gridX = static_cast<size_t>(localX / cellSize_);
            gridY = static_cast<size_t>(localY / cellSize_);

            return gridX < simulation_.getGrid().getWidth() &&
                   gridY < simulation_.getGrid().getHeight();
        }

        void GameUI::renderHelp()
        {
            int panelX = renderer_.getWidth() - 280;
            int panelY = 20;
            int panelW = 260;
            int panelH = 200;

            // Semi-transparent dark panel
            renderer_.renderRect(panelX, panelY, panelW, panelH, Colour(20, 20, 30, 220));
            renderer_.renderRectOutline(panelX, panelY, panelW, panelH, Colour(100, 100, 120));

            // Title
            Colour titleColour(255, 255, 100);
            renderer_.renderText("CONTROLS", panelX + 10, panelY + 10, titleColour);

            // Help text (rendered in console for MVP)
            //  TODO In full version, use SDL_ttf
            static bool printed = false;
            if (!printed)
            {
                std::cout << "\n=== CONTROLS ===\n";
                std::cout << "Left Click:  Ignite cell\n";
                std::cout << "Right Click: Apply water\n";
                std::cout << "SPACE:       Pause/Resume\n";
                std::cout << "R:           Reset\n";
                std::cout << "H:           Toggle help\n";
                std::cout << "ESC:         Quit\n";
                std::cout << "+/-:         Speed control\n";
                std::cout << "================\n\n";
                printed = true;
            }
        }

        void GameUI::renderStats()
        {
            const auto &stats = simulation_.getStats();

            int panelX = 20;
            int panelY = gridOffsetY_ + static_cast<int>(simulation_.getGrid().getHeight()) * cellSize_ + 20;
            int panelW = renderer_.getWidth() - 40;
            int panelH = 140;

            // Panel background
            renderer_.renderRect(panelX, panelY, panelW, panelH, Colour(20, 20, 30, 220));
            renderer_.renderRectOutline(panelX, panelY, panelW, panelH, Colour(100, 100, 120));

            // Draw colored bars for cell counts
            int barHeight = 30;
            int barY = panelY + 20;
            int barSpacing = 10;

            size_t totalCells = stats.getTotalCells();
            if (totalCells > 0)
            {
                int maxBarWidth = panelW - 40;

                // Unburned (green)
                int unburnedWidth = static_cast<int>((stats.unburnedCells * maxBarWidth) / totalCells);
                renderer_.renderRect(panelX + 20, barY, unburnedWidth, barHeight, Colour(50, 150, 50));

                // Burning (red)
                int burningWidth = static_cast<int>((stats.burningCells * maxBarWidth) / totalCells);
                renderer_.renderRect(panelX + 20, barY + barHeight + barSpacing,
                                     burningWidth, barHeight, Colour(255, 50, 50));

                // Burned (gray)
                int burnedWidth = static_cast<int>((stats.burnedCells * maxBarWidth) / totalCells);
                renderer_.renderRect(panelX + 20, barY + 2 * (barHeight + barSpacing),
                                     burnedWidth, barHeight, Colour(80, 80, 80));

                // Wet/Suppressed (blue)
                int wetWidth = static_cast<int>(((stats.wetCells + stats.suppressedCells) * maxBarWidth) / totalCells);
                renderer_.renderRect(panelX + 20, barY + 3 * (barHeight + barSpacing),
                                     wetWidth, barHeight, Colour(50, 150, 255));
            }

            // Console output for stats (since we don't have SDL_ttf) TODo
            static size_t lastStepCount = 0;
            if (stats.stepCount != lastStepCount && stats.stepCount % 10 == 0)
            {
                std::cout << "Step " << stats.stepCount
                          << " | Burning: " << stats.burningCells
                          << " | Burned: " << stats.burnedCells
                          << " | Wet: " << stats.wetCells
                          << " | " << (paused_ ? "PAUSED" : "RUNNING") << "\n";
                lastStepCount = stats.stepCount;
            }
        }

        void GameUI::renderAgents()
        {
            if (agents_.empty())
            {
                return;
            }

            int panelX = renderer_.getWidth() - 280;
            int panelY = 240;
            int panelW = 260;
            int agentHeight = 60;
            int panelH = static_cast<int>(agents_.size()) * agentHeight + 20;

            // Panel background
            renderer_.renderRect(panelX, panelY, panelW, panelH, Colour(20, 20, 30, 220));
            renderer_.renderRectOutline(panelX, panelY, panelW, panelH, Colour(100, 100, 120));

            // Render each agent
            int yPos = panelY + 10;
            for (const auto *agent : agents_)
            {
                // Agent indicator Colour
                Colour agentColour(255, 150, 50);

                // Water bar
                float waterPercent = agent->getWaterRemaining() / agent->getWaterCapacity();
                int barWidth = static_cast<int>((panelW - 40) * waterPercent);
                renderer_.renderRect(panelX + 20, yPos + 30, barWidth, 15, Colour(50, 150, 255));
                renderer_.renderRectOutline(panelX + 20, yPos + 30, panelW - 40, 15, Colour(100, 100, 120));

                yPos += agentHeight;
            }

            // Console output for agents
            static int lastPrint = 0;
            int currentStep = static_cast<int>(simulation_.getStepCount());
            if (currentStep - lastPrint >= 20)
            {
                for (const auto *agent : agents_)
                {
                    std::cout << agent->getName()
                              << " | Water: " << static_cast<int>(agent->getWaterRemaining())
                              << "/" << static_cast<int>(agent->getWaterCapacity())
                              << " | Suppressed: " << agent->getCellsSuppressed() << "\n";
                }
                lastPrint = currentStep;
            }
        }

    } // namespace gui
} // namespace cascade