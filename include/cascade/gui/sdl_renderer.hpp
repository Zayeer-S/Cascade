#ifndef CASCADE_GUI_SDLRENDERER_HPP
#define CASCADE_GUI_SDLRENDERER_HPP

#include <SDL.h>
#include "cascade/grid/Grid.hpp"
#include <string>
#include <sddl.h>

namespace cascade
{
    namespace gui
    {
        /**
         * @brief Colour structure for RGB
         */
        struct Colour
        {
            uint8_t r, g, b, a;

            Colour(uint8_t red = 0, uint8_t green = 0, uint8_t blue = 0, uint8_t alpha = 255) : r(red), g(green), b(blue), a(alpha) {}
        };

        /**
         * @brief SDL-based renderer for fire simulation
         *
         * Handles all rendering of the grid, cells, and the UI elements/
         */
        class SDLRenderer
        {
        public:
            /**
             * @brief Construct renderer
             * @param title Window title
             * @param width Window width
             * @param height Window height
             */
            SDLRenderer(const std::string &title, int width, int height);

            /**
             * @brief Destructor/Clean up SDL resources
             */
            ~SDLRenderer();

            // Prevents copying
            SDLRenderer(const SDLRenderer &) = delete;
            SDLRenderer &operator=(const SDLRenderer &) = delete;

            /**
             * @brief Check if window should close
             */
            bool shouldClose() const { return shouldClose_; }

            /**
             * @brief Clear the screen
             */
            void clear();

            /**
             * @brief Present the rendered frame
             */
            void present();

            /**
             * @brief Render the grid
             * @param grid Grid to render
             * @param cellSize Size of each cell in pixels
             * @param offsetX X offset for grid position
             * @param offsetY Y offset for grid position
             */
            void renderGrid(const Grid &grid, int cellSize, );

            /**
             * @brief Render text
             * @param text Text to render
             * @param x X position
             * @param y Y position
             * @param colour Text colour
             */
            void renderText(const std::string &text, int x, int y, const Colour &colour);

            /**
             * @brief Render outlined rectangle
             * @param x X position
             * @param y Y position
             * @param w Width
             * @param h Height
             * @param colour Outline colour
             */
            void renderRect(int x, int y, int w, int h, const Colour &colour);
        }

        /**
         * @brief Renderr outlined rectangle
         * @param x X position
         * @param y Y position
         * @param w Width
         * @param h Height
         * @param colour Outline colour
         */
        void
        renderRectOutline(int x, int y, int w, int h, const Colour &colour);

        /**
         * @brief Get window width
         */
        int getWidth() const { return width_; }

        /**
         * @brief Get window height
         */
        int getHeight() const { return height_; }

        /**
         * @brief Set close flag
         */
        void setShouldClose(bool close) { shouldClose_ = close; }

        /**
         * @brief Get SDL renderer (for advanced usage)
         */
        SDL_Renderer *getSDLRenderer() { return renderer_; }

    private:
        /**
         * @brief Get colour for cell state
         * @param cell Cell to get colour for
         * @return Colour for the cell
         */
        Colour getCellColour(const Cell &cell) const;

        /**
         * @brief Interpolate between two colours
         * @param c1 First colour
         * @param c2 Second colour
         * @param t Interpolation factor (0-1)
         * @return Interpolated colour
         */
        Color lerpColour(const Colour &c1, const Colour &c2, float t) const;

        SDL_Window *window_;
        SDL_Renderer *renderer_;
        int width_;
        int height_;
        bool shouldClose_;
    } // namespace gui
} // namespace cascade

#endif