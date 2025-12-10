#include "cascade/suppression/suppression_agent.hpp"
#include "cascade/core/simulation.hpp"
#include <cmath>

namespace cascade
{
    SuppressionAgent::SuppressionAgent(const std::string &name, float waterCapacity, float waterPerCell)
        : name_(name),
          waterCapacity_(waterCapacity),
          waterRemaining_(waterCapacity),
          waterPerCell_(waterPerCell),
          cellsSuppressed_(0),
          active_(true)
    {
    }

    void SuppressionAgent::onSimulationStep(const Simulation &simulation, size_t stepNumber)
    {
        (void)stepNumber;

        if (!active_ || !hasWater())
        {
            return;
        }

        Grid &grid = const_cast<Grid &>(simulation.getGrid());

        std::vector<Cell *> targets = selectTargets(grid);

        for (Cell *cell : targets)
        {
            if (!hasWater())
            {
                break;
            }

            applyWater(cell);
        }
    }

    bool SuppressionAgent::applyWater(Cell *cell)
    {
        if (!cell || waterRemaining_ <= 0.0f)
        {
            return false;
        }

        float waterToUse = std::min(waterPerCell_, waterRemaining_);
        cell->extinguish(waterToUse);

        waterRemaining_ -= waterToUse;
        cellsSuppressed_++;

        return true;
    }

    std::vector<Cell *> SuppressionAgent::getCellsInRadius(Grid &grid, const Vector2D &center, float radius)
    {
        std::vector<Cell *> result;

        int minX = std::max(0, static_cast<int>(center.x - radius));
        int maxX = std::min(static_cast<int>(grid.getWidth()) - 1, static_cast<int>(center.x + radius));
        int minY = std::max(0, static_cast<int>(center.y - radius));
        int maxY = std::min(static_cast<int>(grid.getHeight()) - 1, static_cast<int>(center.y + radius));

        float radiusSq = radius * radius;

        for (int y = minY; y <= maxY; ++y)
        {
            for (int x = minX; x <= maxX; ++x)
            {
                Vector2D cellPos(static_cast<float>(x), static_cast<float>(y));
                float distSq = (cellPos - center).lengthSquared();

                if (distSq <= radiusSq)
                {
                    result.push_back(&grid.getCell(static_cast<size_t>(x), static_cast<size_t>(y)));
                }
            }
        }

        return result;
    }

    void SuppressionAgent::reset()
    {
        waterRemaining_ = waterCapacity_;
        cellsSuppressed_ = 0;
        active_ = true;
    }
} // namespace cascade
