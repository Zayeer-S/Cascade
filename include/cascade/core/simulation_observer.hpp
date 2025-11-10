#ifndef CASCADE_CORE_SIMULATIONOBSERVER_HPP
#define CASCADE_CORE_SIMULATIONOBSERVER_HPP

#include <cstddef>

namespace cascade
{
    class Simulation;
    class Grid;

    /**
     * @brief Observer interface for simulation events
     * 
     * This interface receives notifications about simulation stage changes, steps, and completion.
     */
    class SimulationObserver {
        public:
        virtual ~SimulationObserver()=default;

        /**
         * @brief Called when simulation is initailzed/reset
         * @param simulation Reference to the simulation
         */
        virtual void onSimulationStart(const Simulation& simulation) {
            (void)simulation;
        }

        /**
         * @brief Called after each simulation step
         * @param simulation Reference to the simulation
         * @param stepNumber Current step number
         */
        virtual void onSimulationStep(const Simulation& simulation, size_t stepNumber) {
            (void)simulation;
            (void)stepNumber;
        }

        /**
         * @brief Called when simulation ends or is stopped
         * @param simulation Reference to the simulation
         * @param reason Reason for completion
         */
        virtual void onSimulationEnd(const Simulation& simulation, const char* reason) {
            (void)simulation;
            (void)reason;
         }

         /**
          * @brief Called when a cell ignites
          * @param x X co-ord of ignited cell
          * @param y Y co-ord of ignited cell
          */
         virtual void onCellIgnited(size_t x, size_t y) {
            (void)x;
            (void)y;
         }

         /**
          * @brief Called when a cell burns out
          * @param x X co-ord of burned cell
          * @param y Y co-ord of burned cell
          */
         virtual void onCellBurnedOut(size_t x, size_t y) {
            (void)x;
            (void)y;
         }
    };
} // namespace cascade

#endif