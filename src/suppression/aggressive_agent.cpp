#include "cascade/suppression/aggressive_agent.hpp"
#include <algorithm>
#include <cmath>

namespace cascade
{
    AggressiveAgent::AggressiveAgent(const std::string &name, float waterCapacity, float waterPerCell) : SuppressionAgent(name, waterCapacity, waterPerCell), position_(0.0f, 0.0f), hasPosition_(false), maxRadius_(1000.0f)
    {
    }

    std::vector<Cell *> AggressiveAgent::selectTargets(Grid &grid)
    {
        std::vector<Cell *> burningCells = grid.getCellsByState(CellState::Burning);

        if (hasPosition_)
        {
            burningCells.erase(
                std::remove_if(burningCells.begin(), burningCells.end(),
                               [this](const Cell *cell)
                               {
                                   Vector2D cellPos(static_cast<float>(cell->getX()),
                                                    static_cast<float>(cell->getY()));
                                   float dist = (cellPos - position_).length();
                                   return dist > maxRadius_;
                               }),
                burningCells.end());
        }

        std::sort(burningCells.begin(), burningCells.end(), [this](const Cell *a, const Cell *b)
                  { return calculatePriority(a) > calculatePriority(b); });

        size_t maxCells = static_cast<size_t>(waterRemaining_ / waterPerCell_);
        if (burningCells.size() > maxCells)
        {
            burningCells.resize(maxCells);
        }

        return burningCells;
    }

    float AggressiveAgent::calculatePriority(const Cell *cell) const
    {
        float priority = 0.0f;

        priority += cell->getIntensity() * 40.0f;
        priority += cell->getFuelAmount() * 30.0f;

        float tempFactor = std::min(cell->getTemperatureAmount() / 1000.0f, 1.0f);
        priority += tempFactor * 20.0f;

        if (hasPosition_)
        {
            Vector2D cellPos(static_cast<float>(cell->getX()),
                             static_cast<float>(cell->getY()));
            float distance = (cellPos - position_).length();
            float proximityFactor = std::max(0.0f, 1.0f - (distance / maxRadius_));
            priority += proximityFactor * 10.0f;
        }
        else
        {
            priority += 10.0f;
        }

        return priority;
    }
} // namespace cascade
