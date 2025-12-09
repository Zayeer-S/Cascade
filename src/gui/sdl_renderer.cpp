#include "cascade/gui/sdl_renderer.hpp"
#include <stdexcept>
#include <algorithm>
#include <cmath>

namespace cascade {
namespace gui {

SDLRenderer::SDLRenderer(const std::string& title, int width, int height)
    : window_(nullptr)
    , renderer_(nullptr)
    , width_(width)
    , height_(height)
    , shouldClose_(false)
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw std::runtime_error(std::string("SDL_Init failed: ") + SDL_GetError());
    }

    // Create window
    window_ = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_SHOWN
    );

    if (!window_) {
        SDL_Quit();
        throw std::runtime_error(std::string("SDL_CreateWindow failed: ") + SDL_GetError());
    }

    // Create renderer
    renderer_ = SDL_CreateRenderer(
        window_,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (!renderer_) {
        SDL_DestroyWindow(window_);
        SDL_Quit();
        throw std::runtime_error(std::string("SDL_CreateRenderer failed: ") + SDL_GetError());
    }

    // Enable alpha blending
    SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_BLEND);
}

SDLRenderer::~SDLRenderer() {
    if (renderer_) {
        SDL_DestroyRenderer(renderer_);
    }
    if (window_) {
        SDL_DestroyWindow(window_);
    }
    SDL_Quit();
}

void SDLRenderer::clear() {
    SDL_SetRenderDrawColor(renderer_, 20, 20, 30, 255); // Dark background
    SDL_RenderClear(renderer_);
}

void SDLRenderer::present() {
    SDL_RenderPresent(renderer_);
}

void SDLRenderer::renderGrid(const Grid& grid, int cellSize, int offsetX, int offsetY) {
    for (size_t y = 0; y < grid.getHeight(); ++y) {
        for (size_t x = 0; x < grid.getWidth(); ++x) {
            const Cell& cell = grid.getCell(x, y);
            Colour color = getCellColour(cell);

            int px = offsetX + static_cast<int>(x) * cellSize;
            int py = offsetY + static_cast<int>(y) * cellSize;

            // Fill cell
            SDL_Rect rect = { px, py, cellSize, cellSize };
            SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a);
            SDL_RenderFillRect(renderer_, &rect);

            // Draw grid lines
            SDL_SetRenderDrawColor(renderer_, 40, 40, 50, 100);
            SDL_RenderDrawRect(renderer_, &rect);
        }
    }
}

void SDLRenderer::renderText(const std::string& text, int x, int y, const Colour& color) {
    // For MVP, we'll skip text rendering (requires SDL_ttf)
    // In a full version, you'd use SDL_ttf to render fonts
    // For now, text is rendered as simple boxes with labels in console
    (void)text;
    (void)x;
    (void)y;
    (void)color;
}

void SDLRenderer::renderRect(int x, int y, int w, int h, const Colour& color) {
    SDL_Rect rect = { x, y, w, h };
    SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer_, &rect);
}

void SDLRenderer::renderRectOutline(int x, int y, int w, int h, const Colour& color) {
    SDL_Rect rect = { x, y, w, h };
    SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a);
    SDL_RenderDrawRect(renderer_, &rect);
}

Colour SDLRenderer::getCellColour(const Cell& cell) const {
    switch (cell.getState()) {
        case CellState::Unburned:
            // Green gradient based on fuel
            {
                float fuel = cell.getFuelAmount();
                uint8_t green = static_cast<uint8_t>(40 + fuel * 100);
                return Colour(20, green, 20);
            }

        case CellState::Igniting:
            // Orange
            return Colour(255, 140, 0);

        case CellState::Burning:
            // Red-yellow gradient based on intensity
            {
                float intensity = cell.getIntensity();
                Colour red(255, 0, 0);
                Colour yellow(255, 255, 0);
                return lerpColour(red, yellow, intensity);
            }

        case CellState::Smouldering:
            // Dark red
            return Colour(139, 0, 0);

        case CellState::Burned:
            // Dark gray/black
            return Colour(30, 30, 30);

        case CellState::Wet:
            // Cyan/blue gradient based on moisture
            {
                float moisture = cell.getMoisture();
                uint8_t blue = static_cast<uint8_t>(100 + moisture * 155);
                return Colour(0, 180, blue);
            }

        case CellState::Suppressed:
            // Dark blue
            return Colour(0, 100, 150);

        default:
            return Colour(128, 128, 128);
    }
}

Colour SDLRenderer::lerpColour(const Colour& c1, const Colour& c2, float t) const {
    t = std::clamp(t, 0.0f, 1.0f);
    
    return Colour(
        static_cast<uint8_t>(c1.r + (c2.r - c1.r) * t),
        static_cast<uint8_t>(c1.g + (c2.g - c1.g) * t),
        static_cast<uint8_t>(c1.b + (c2.b - c1.b) * t),
        static_cast<uint8_t>(c1.a + (c2.a - c1.a) * t)
    );
}

} // namespace gui
} // namespace cascade