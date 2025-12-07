#include "cascade/suppression/defensive_agent.hpp"
#include <algorithm>
#include <cmath>

namespace cascade
{
    DefensiveAgent::DefensiveAgent(const std::string &name, float waterCapacity, float waterPerCell) : SuppressionAgent(name, waterCapacity, waterPerCell), protectedZone_(0.0f, 0.0f), protectionRadius_(10.0f), hasProtectedZone_(false), lookahead_(2)
    {
    }

    std::vector<Cell *> DefensiveAgent::selectTargets(Grid &grid)
    {
        std::vector<Cell *> candidates;

        grid.forEachIf(
            [](const Cell &c)
            { return c.getState() == CellState::Unburned; },
            [&candidates, &grid, this](Cell &c)
            {
                bool nearFire = false;

                for (size_t radius = 1; radius <= lookahead_; ++radius)
                {
                    auto neighbours = grid.getCellsInRadius(c.getX(), c.getY(), static_cast<float>(radius));
                    for (const Cell *neighbour : neighbours)
                    {
                        if (neighbour->isBurning())
                        {
                            nearFire = true;
                            break;
                        }
                    }
                    if (nearFire)
                        break;
                }

                if (nearFire)
                    candidates.push_back(&c);
            });

        std::sort(candidates.begin(), candidates.end(), [this, &grid](const Cell *a, const Cell *b)
                  { return calculateFirebreakPriority(a, grid) > calculateFirebreakPriority(b, grid); });

        size_t maxCells = static_cast<size_t>(waterRemaining_ / waterPerCell_);
        if (candidates.size() > maxCells)
        {
            candidates.resize(maxCells);
        }

        return candidates;
    }

    float DefensiveAgent::calculateFirebreakPriority(const Cell *cell, const Grid &grid) const
    {
        float priority = 0.0f;

        size_t burningNeighbours = countBurningNeighbours(cell, grid);
        priority += (burningNeighbours / 8.0f) * 40.0f;

        priority += cell->getFuelAmount() * 30.0f;

        if (hasProtectedZone_)
        {
            Vector2D cellPos(static_cast<float>(cell->getX()), static_cast<float>(cell->getY()));
            float distance = (cellPos - protectedZone_).length();

            if (distance <= protectionRadius_)
            {
                priority += 20.0f;
            }
            else
            {
                float proximityFactor = std::max(0.0f, 1.0f - ((distance - protectionRadius_) / protectionRadius_));
                priority += proximityFactor * 20.0f;
            }
        }
        else
        {
            priority += 10.0f;
        }

        float dryness = 1.0f - cell->getMoisture();
        priority += dryness * 10.0f;

        return priority;
    }

    size_t DefensiveAgent::countBurningNeighbours(const Cell *cell, const Grid &grid) const
    {
        auto neighbors = grid.getNeighbours8(cell->getX(), cell->getY());

        size_t count = 0;
        for (const Cell *neighbor : neighbors)
        {
            if (neighbor->isBurning())
            {
                count++;
            }
        }

        return count;
    }
} // namespace cascade