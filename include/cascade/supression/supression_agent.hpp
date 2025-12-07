#ifndef CASCADE_SUPRESSION_SUPRESSIONAGENT_HPP
#define CASCADE_SUPRESSION_SUPRESSIONAGENT_HPP

#include "cascade/core/simulation_observer.hpp"
#include "cascade/grid/Grid.hpp"
#include "cascade/utils/Vector2D.hpp"
#include <vector>
#include <string>

namespace cascade
{
    /**
     * @brief Abstract base class for fire supression agents
     *
     * Supression agents observe the simulation and apply water to cells to control fire spread.
     */
    class SupressionAgent : public SimulationObserver
    {
    public:
        /**
         * @brief Construct agent with resource capacity
         * @param name Agent name for identification
         * @param waterCapacity Total water available (liters)
         * @param waterPerCell Amount of water per cell application
         */
        SupressionAgent(const std::string &name, float waterCapacity, float waterPerCell = 0.5f);

        virtual ~SupressionAgent() = default;

        /**
         * @brief Called each step to decide and apply suppression
         * @param simulation The simulation being observed
         * @param stepNumber Current step number
         */
        void onSimulationStep(const Simulation &simulation, size_t stepNumber) override;

        /**
         * @brief Get agent name
         */
        const std::string &getName() const { return name_; }

        /**
         * @brief Get current water level
         */
        float getWaterRemaining() const { return waterRemaining_; }

        /**
         * @brief Get water capacity
         */
        float getWaterCapacity() const { return waterCapacity_; }

        /**
         * @brief Check if agent has water remaining
         */
        bool hasWater() const { return waterRemaining_ > 0.0f; }

        /**
         * @brief Get number of cells suppressed
         */
        size_t getCellsSuppressed() const { return cellsSuppressed_; }

        /**
         * @brief Reset agent state
         */
        virtual void reset();

    protected:
        /**
         * @brief Strategy method - select target cells for suppression
         * @param grid Current grid state
         * @return List of cell pointers to suppress (priority order)
         */
        virtual std::vector<Cell *> selectTargets(Grid &grid) = 0;

        /**
         * @brief Apply water to a cell
         * @param cell Target cell
         * @return true if water was applied
         */
        bool applyWater(Cell *cell);

        /**
         * @brief Get cells in radius around a point
         * @param grid The grid
         * @param center Center position
         * @param radius Search radius
         * @return Cells within radius
         */
        std::vector<Cell *> getCellsInRadius(Grid &grid, const Vector2D &center, float radius);

        std::string name_;
        float waterCapacity_;
        float waterRemaining_;
        float waterPerCell_;
        size_t cellsSuppressed_;
        bool active_;
    };

} // namespace cascade

#endif