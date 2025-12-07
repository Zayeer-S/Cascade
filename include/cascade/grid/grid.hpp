#ifndef CASCADE_GRID_GRID_HPP
#define CASCADE_GRID_GRID_HPP

#include "cell.hpp";
#include <vector>
#include <functional>

namespace cascade
{
    /**
     * @brief 2D grid container for simulation cells
     *
     * Manages a rectangular grid of cells with wefficient neighbour queries and iteration utilities.
     */

    class Grid
    {
    public:
        /**
         * @brief Construct a grid with specified dimensions
         * @param width number of cells in x dimension
         * @param heigh number of cells in y dimension
         */
        Grid(size_t width, size_t height);

        /**
         * @brief Get cell at a specified position (w/ bounds checking)
         * @param x X co-ord
         * @param y Y co-ord
         * @return Reference to cell
         * @throws std::out_of_range if co-ords are invalid
         */
        Cell &getCell(size_t x, size_t y);

        /**
         * @brief Get cell at specified position (const version)
         * @param x X co-ord
         * @param y Y co-ord
         * @return Const reference to cell
         * @throws std::out_of_range if coordinates are invalid
         */
        const Cell &getCell(size_t x, size_t y) const;

        /**
         * @brief Get cell at position without bounds checking (faster)
         * @param x X co-ord
         * @param y Y co-ord
         * @return Reference to cell
         * @warning No bounds checking - caller must ensure valid coordinates
         */
        Cell &getCellUnsafe(size_t x, size_t y)
        {
            return cells_[y][x];
        }

        /**
         * @brief Check if coordinates are within grid bounds
         * @param x X co-ord
         * @param y Y co-ord
         * @return true if coordinates are valid
         */
        bool isValid(size_t x, size_t y) const
        {
            return x < width_ && y < height_;
        }

        /**
         * @brief Get 4-connected neighbhours (N, S, E, W)
         * @param x X co-ord
         * @param y Y co-ord
         * @return Vector of pointers to neighbhour cells
         */
        std::vector<Cell *> getNeighbhours4(size_t x, size_t y);
        std::vector<const Cell *> getNeighbhours4(size_t x, size_t y) const;

        /**
         * @brief Get 8-connected neighbhours (includes diagonals)
         * @param x X co-ord
         * @param y Y co-ord
         * @return Vector of pointers to neighbhour cells
         */
        std::vector<Cell *> getNeighbhours8(size_t x, size_t y);
        std::vector<const Cell *> getNeighbhours8(size_t x, size_t y) const;

        /**
         * @brief Get all cells within specified radius
         * @param x Center X co-ord
         * @param y Center Y co-ord
         * @param radius Radius in cells (uses Manhattan distance)
         * @return Vector of pointers to cells within radius
         */
        std::vector<Cell *> getCellsInRadius(size_t x, size_t y, float radius);

        /**
         * @brief Apply function to all cells
         * @param func Function to apply to each cell
         */
        void forEach(std::function<void(Cell &)> func);

        /**
         * @brief Apply function to all cells (const version)
         * @param func Function to apply to each cell
         */
        void forEach(std::function<void(const Cell &)> func) const;

        /**
         * @brief Apply function to cells matching predicate
         * @param predicate Function returning true for cells to process
         * @param func Function to apply to matching cells
         */
        void forEachIf(std::function<bool(const Cell &)> predicate,
                       std::function<void(Cell &)> func);

        /**
         * @brief Count cells matching predicate
         * @param predicate Function returning true for cells to count
         * @return Number of matching cells
         */
        size_t countIf(std::function<bool(const Cell &)> predicate) const;

        /**
         * @brief Get all cells in a specific state
         * @param state State to search for
         * @return Vector of pointers to cells in that state
         */
        std::vector<Cell *> getCellsByState(CellState state);

        void reset();

        size_t getWidth() const
        {
            return width_;
        }
        size_t getHeight() const
        {
            return height_;
        }
        size_t getTotalCells() const
        {
            return width_ * height_;
        }

    private:
        std::vector<std::vector<Cell>> cells_;
        size_t width_;
        size_t height_;

        void addNeighbourIfValid(std::vector<Cell *> &neighbhours, int x, int y);
    };
} // namespace cascade

#endif