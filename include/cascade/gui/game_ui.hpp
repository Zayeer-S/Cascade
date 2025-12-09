#ifndef CASCADE_GUI_GAMEUI_HPP
#define CASCADE_GUI_GAMEUI_HPP

#include "sdl_renderer.hpp"
#include "cascade/core/simulation.hpp"
#include "cascade/suppression/aggressive_agent.hpp"
#include "cascade/suppression/defensive_agent.hpp"
#include <SDL2/SDL.h>
#include <memory>
#include <vector>

namespace cascade
{
    namespace gui
    {
        /**
         * @brief Game UI and interaction manager
         *
         * Handles user input, UI rendering, and game state
         */
        class GameUI
        {
        public:
            /**
             * @brief Construct game UI
             * @param renderer SDL renderer
             * @param simulation Simulation instance
             */
            GameUI(SDLRenderer &renderer, Simulation &simulation);

            /**
             * @brief Process SDL events
             * @return false if should quit
             */
            bool processEvents();

            /**
             * @brief Update UI state
             * @param deltaTime Time since last update
             */
            void update(float deltaTime);

            /**
             * @brief Render UI
             */
            void render();

            /**
             * @brief Check if simulation is paused
             */
            bool isPaused() const { return paused_; }

            /**
             * @brief Set pause state
             */
            void setPaused(bool paused) { paused_ = paused; }

            /**
             * @brief Get cell size
             */
            int getCellSize() const { return cellSize_; }

            /**
             * @brief Add agent to UI tracking
             */
            void addAgent(SuppressionAgent *agent);

        private:
            /** @brief Handle mouse click
             *@param x Mouse X
             *@param y Mouse Y
             *@param button Mouse button
             */
            void handleMouseClick(int x, int y, int button);

            /**
             * @brief Handle keyboard input
             * @param key Key code
             */
            void handleKeyPress(SDL_Keycode key);

            /**
             * @brief Convert screen coordinates to grid coordinates
             * @param screenX Screen X
             * @param screenY Screen Y
             * @param gridX Output grid X
             * @param gridY Output grid Y
             * @return true if valid grid position
             */
            bool screenToGrid(int screenX, int screenY, size_t &gridX, size_t &gridY);

            /**
             * @brief Render help text
             */
            void renderHelp();

            /**
             * @brief Render statistics panel
             */
            void renderStats();

            /**
             * @brief Render agent info
             */
            void renderAgents();

            SDLRenderer &renderer_;
            Simulation &simulation_;
            std::vector<SuppressionAgent *> agents_;

            bool paused_;
            int cellSize_;
            int gridOffsetX_;
            int gridOffsetY_;

            bool showHelp_;
            int mouseGridX_;
            int mouseGridY_;
        }
    } // namespace gui

} // namespace cascade

#endif