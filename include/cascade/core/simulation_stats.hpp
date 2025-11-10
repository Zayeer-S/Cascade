#ifndef CASCADE_CORE_SIMULATIONSTATS_HPP
#define CASCADE_CORE_SIMULATIONSTATS_HPP

#include <cstddef>

namespace cascade
{
    /**
     * @brief Statistics tracking for simulation
     *
     * Tracks metrics about the simulation state:
     * Cell counts by state
     * Total cells affected
     * Simulation time
     * Step count
     */
    struct SimulationStats
    {
        size_t unburnedCells;
        size_t burningCells;
        size_t burnedCells;
        size_t wetCells;
        size_t supressedCells;

        size_t totalCellsIgnited;
        size_t totalCellsBurnedOut;

        float simulationTime;
        size_t stepCount;

        float percentBurned;
        float averageStepTime;

        /**
         * @brief Construct statistics with zeros/empty stats
         */
        SimulationStats()
            : unburnedCells(0), burningCells(0), burnedCells(0), wetCells(0), supressedCells(0), totalCellsIgnited(0), totalCellsBurnedOut(0), simulationTime(0.0f), stepCount(0), percentBurned(0.0f), averageStepTime(0.0f)
        {
        }

        /**
         * @brief Reset all stats to zero
         */
        void reset()
        {
            *this=SimulationStats();
        }

        /**
         * @brief Get total number of cells
         */
        size_t getTotalCells() const {
            return unburnedCells + burningCells + burnedCells + wetCells + supressedCells;
        }

        /**
         * @brief Get total active fire cells (burning + smouldering)
         */
        size_t getActiveFire() const {
            return burningCells;
        }

        /**
         * @brief Check if fire is stil active
         */
        bool isFireActive() const {
            return burningCells > 0;
        }
    };

} // namespace cascade

#endif