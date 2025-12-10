#ifndef CASCADE_CORE_SIMULATION_HPP
#define CASCADE_CORE_SIMULATION_HPP

#include "cascade/grid/grid.hpp"
#include "cascade/fire/fire_model.hpp"
#include "cascade/environment/environment.hpp"
#include "cascade/core/time.hpp"
#include "cascade/core/simulation_stats.hpp"
#include "cascade/core/simulation_observer.hpp"
#include <memory>
#include <vector>
#include <chrono>

namespace cascade
{
    /**
     * @brief Main simulation orchestrator
     *
     * Simulation class co-ordinates all components and is the main entry point for running a simulation.
     */
    class Simulation
    {
    public:
        /**
         * @brief Construct simulation with grid dimensions
         * @param width Grid width in cells
         * @param height Grid height in cells
         */
        Simulation(size_t width, size_t height);

        /**
         * @brief Destructor
         */
        ~Simulation() = default;

        Simulation(const Simulation &) = delete;
        Simulation &operator=(const Simulation &) = delete;

        Simulation(Simulation &&) = default;
        Simulation &operator=(Simulation &&) = default;

        /**
         * @brief Initialize simulation with fire model
         * @param fireModel Fire spread model (takes ownership)
         */
        void initialize(std::unique_ptr<FireModel> fireModel_);

        /**
         * @brief Reset simulation to initial state
         */
        void reset();

        /**
         * @brief Execute one simulation step
         * @param deltaTime Time step in seconds (default: 0.1)
         * @return true if simulation is still active
         */
        bool step(float deltaTime = 0.1f);

        /**
         * @brief Run simulation for specified duration
         * @param maxTime Maximum simulation time (seconds)
         * @param deltaTime Time step per iteration (seconds)
         * @return Number of steps executed
         */
        size_t run(float maxTime, float deltaTime = 0.1f);

        /**
         * @brief Run simulation until fire burns out or max steps
         * @param maxSteps Maximum steps to execute (0 = unlimited)
         * @param deltaTime Time step per iteration (seconds)
         * @return Number of steps executed
         */
        size_t runUntilComplete(size_t maxSteps = 0, float deltaTime = 0.1f);

        void addObserver(SimulationObserver *observer);
        void removeObserver(SimulationObserver *observer);
        void notifyObservers();

        Grid &getGrid() { return grid_; }
        const Grid &getGrid() const { return grid_; }

        Environment &getEnvironment() { return environment_; }
        const Environment &getEnvironment() const { return environment_; }

        FireModel *getFireModel() { return fireModel_.get(); }
        const FireModel *getFireModel() const { return fireModel_.get(); }

        const SimulationStats &getStats() const { return stats_; }

        const Time &getTime() const { return time_; }

        float getSimulationTime() const { return stats_.simulationTime; }
        size_t getStepCount() const { return stats_.stepCount; }

        bool isRunning() const { return running_; }
        bool isFireActive() const { return stats_.isFireActive(); }

        /**
         * @brief Stops the simulation
         */
        void stop() { running_ = false; }

        void refresh() { updateStats(); }

    private:
        void updateStats();
        void detectCellChanges();

        Grid grid_;
        Environment environment_;
        std::unique_ptr<FireModel> fireModel_;
        Time time_;

        SimulationStats stats_;
        std::vector<SimulationObserver *> observers_;
        bool running_;
        bool initialized_;

        std::vector<CellState> previousStates_;

        std::chrono::high_resolution_clock::time_point lastStepTime_;
    };
} // namespace cascade

#endif