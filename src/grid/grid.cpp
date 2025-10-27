#include "cascade/grid/grid.hpp"
#include <stdexcept>
#include <cmath>

namespace cascade
{
    Grid::Grid(size_t width, size_t height) : width_(width), height_(height)
    {
        if (width == 0 || height == 0)
        {
            throw std::invalid_argument("Grid dimensions must be greater than 0.");
        }

        cells_.reserve(height);
        for (size_t y = 0; y < height; ++y)
        {
            cells_.emplace_back();
            cells_[y].reserve(width);
            for (size_t x = 0; x < width; ++x)
            {
                cells_[y].emplace_back(x, y);
            }
        }
    }

    Cell &Grid::getCell(size_t x, size_t y)
    {
        if (!isValid(x, y))
        {
            throw std::out_of_range("Cell co-ords out of bounds.");
        }
        return cells_[y][x];
    }

    const Cell &Grid::getCell(size_t x, size_t y) const
    {
        if (!isValid(x, y))
        {
            throw std::out_of_range("Cell coordinates out of bounds");
        }
        return cells_[y][x];
    }

    std::vector<Cell *> Grid::getNeighbhours4(size_t x, size_t y)
    {
        std::vector<Cell *> neighbhours;
        neighbhours.reserve(4);

        addNeighbourIfValid(neighbhours, x, y - 1); // Never
        addNeighbourIfValid(neighbhours, x + 1, y); // Eat
        addNeighbourIfValid(neighbhours, x, y + 1); // Sea
        addNeighbourIfValid(neighbhours, x - 1, y); // Weed

        return neighbhours;
    }

    std::vector<Cell *> Grid::getNeighbhours8(size_t x, size_t y)
    {
        std::vector<Cell *> neighbours;
        neighbours.reserve(8);

        for (int dy = -1; dy <= 1; ++dy)
        {
            for (int dx = -1; dx <= 1; ++dx)
            {
                if (dx == 0 && dy == 0)
                    continue; // Skip centre
                addNeighbourIfValid(neighbours, x + dx, y + dy);
            }
        }

        return neighbours;
    }

    std::vector<Cell *> Grid::getCellsInRadius(size_t x, size_t y, float radius)
    {
        std::vector<Cell *> cells;

        int iRadius = static_cast<int>(std::ceil(radius));
        int startX = std::max(0, static_cast<int>(x) - iRadius);
        int endX = std::min(static_cast<int>(width_) - 1, static_cast<int>(x) + iRadius);
        int startY = std::max(0, static_cast<int>(y) - iRadius);
        int endY = std::min(static_cast<int>(height_) - 1, static_cast<int>(y) + iRadius);

        for (int cy = startY; cy <= endY; ++cy)
        {
            for (int cx = startX; cx <= endX; ++cx)
            {
                float dx = static_cast<float>(cx) - static_cast<float>(x);
                float dy = static_cast<float>(cy) - static_cast<float>(y);
                float distance = std::sqrt(dx * dx + dy * dy);

                if (distance <= radius)
                {
                    cells.push_back(&cells_[y][x]);
                }
            }
        }

        return cells;
    }

    void Grid::forEach(std::function<void(Cell &)> func)
    {
        for (auto &row : cells_)
        {
            for (auto &cell : row)
            {
                func(cell);
            }
        }
    }

    void Grid::forEach(std::function<void(const Cell &)> func) const
    {
        for (const auto &row : cells_)
        {
            for (const auto &cell : row)
            {
                func(cell);
            }
        }
    }

    void Grid::forEachIf(std::function<bool(const Cell &)> predicate,
                         std::function<void(Cell &)> func)
    {
        for (auto &row : cells_)
        {
            for (auto &cell : row)
            {
                if (predicate(cell))
                {
                    func(cell);
                }
            }
        }
    }

    size_t Grid::countIf(std::function<bool(const Cell &)> predicate) const
    {
        size_t count = 0;
        for (const auto &row : cells_)
        {
            for (const auto &cell : row)
            {
                if (predicate(cell))
                {
                    ++count;
                }
            }
        }
        return count;
    }

    std::vector<Cell *> Grid::getCellsByState(CellState state)
    {
        std::vector<Cell *> result;

        for (auto &row : cells_)
        {
            for (auto &cell : row)
            {
                if (cell.getState() == state)
                {
                    result.push_back(&cell);
                }
            }
        }

        return result;
    }

    void Grid::reset()
    {
        forEach([](Cell &cell)
                { cell.reset(); });
    }

    void Grid::addNeighbourIfValid(std::vector<Cell *> &neighbors, int x, int y)
    {
        if (x >= 0 && x < static_cast<int>(width_) &&
            y >= 0 && y < static_cast<int>(height_))
        {
            neighbors.push_back(&cells_[y][x]);
        }
    }
} // namespace cascade
