#include "cascade/core/simulation.hpp"
#include <algorithm>
#include <stdexcept>

namespace cascade
{
    Simulation::Simulation(size_t width, size_t height) : grid_(width, height), environment_(), fireModel_(nullptr), time_(), stats_(), observers_(), running_(false), initialized_(false), previousStates_(), lastStepTime_(std::chrono::high_resolution_clock::now())
    {
        previousStates_.resize(grid_.getTotalCells(), CellState::Unburned);
    }

    void Simulation::initialize(std::unique_ptr<FireModel> fireModel)
    {
        fireModel_ = std::move(fireModel);

        if (!fireModel_)
        {
            throw std::invalid_argument("fireModel cannot be null");
        }

        reset();
        initialized_ = true;

        for (auto *observer : observers_)
        {
            observer->onSimulationStart(*this);
        }
    }

    void Simulation::reset()
    {
        grid_.reset();
        environment_.reset();

        if (fireModel_)
        {
            fireModel_->reset();
        }

        time_.reset();
        stats_.reset();
        running_ = false;

        std::fill(previousStates_.begin(), previousStates_.end(), CellState::Unburned);

        updateStats();
    }

    bool Simulation::step(float deltaTime)
    {
        if (!initialized_ || !fireModel_)
        {
            return false;
        }

        auto stepStart = std::chrono::high_resolution_clock::now();

        running_ = true;

        fireModel_->update(grid_, environment_, deltaTime);

        grid_.forEach([deltaTime](Cell &cell)
                      { cell.update(deltaTime); });

        environment_.update(deltaTime);

        time_.update();

        detectCellChanges();

        stats_.simulationTime += deltaTime;
        stats_.stepCount++;
        updateStats();

        auto stepEnd = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stepEnd - stepStart);
        float stepTimeMs = duration.count() / 1000.0f;

        if (stats_.stepCount == 1)
        {
            stats_.averageStepTime = stepTimeMs;
        }
        else
        {
            stats_.averageStepTime = (stats_.averageStepTime * 0.95f) + (stepTimeMs * 0.05f);
        }

        lastStepTime_ = stepEnd;

        for (auto *observer : observers_)
        {
            observer->onSimulationStep(*this, stats_.stepCount);
        }

        bool active = stats_.isFireActive();
        if (!active)
        {
            running_ = false;
            for (auto *observer : observers_)
            {
                observer->onSimulationEnd(*this, "burned_out");
            }
        }

        return active;
    }

    size_t Simulation::run(float maxTime, float deltaTime)
    {
        if (!initialized_)
        {
            return 0;
        }

        size_t steps = 0;
        float elapsed = 0.0f;

        while (elapsed < maxTime && step(deltaTime))
        {
            elapsed += deltaTime;
            steps++;
        }

        return steps;
    }

    size_t Simulation::runUntilComplete(size_t maxSteps, float deltaTime)
    {
        if (!initialized_)
        {
            return 0;
        }

        size_t steps = 0;

        while ((maxSteps == 0 || steps < maxSteps) && step(deltaTime))
        {
            steps++;
        }

        if (maxSteps > 0 && steps >= maxSteps)
        {
            running_ = false;
            for (auto *observer : observers_)
            {
                observer->onSimulationEnd(*this, "max_steps_reached");
            }
        }

        return steps;
    }

    void Simulation::addObserver(SimulationObserver *observer)
    {
        if (observer && std::find(observers_.begin(), observers_.end(), observer) == observers_.end())
        {
            observers_.push_back(observer);
        }
    }

    void Simulation::removeObserver(SimulationObserver *observer)
    {
        auto it = std::find(observers_.begin(), observers_.end(), observer);
        if (it != observers_.end())
        {
            observers_.erase(it);
        }
    }

    void Simulation::updateStats()
    {
        stats_.unburnedCells = grid_.countIf([](const Cell &c)
                                             { return c.getState() == CellState::Unburned; });

        stats_.burningCells = grid_.countIf([](const Cell &c)
                                            { return c.isBurning(); });

        stats_.burnedCells = grid_.countIf([](const Cell &c)
                                           { return c.getState() == CellState::Burned; });

        stats_.wetCells = grid_.countIf([](const Cell &c)
                                        { return c.getState() == CellState::Wet; });

        stats_.supressedCells = grid_.countIf([](const Cell &c)
                                               { return c.getState() == CellState::Supressed; });

        stats_.totalCellsBurnedOut = stats_.burnedCells + stats_.supressedCells;

        size_t totalCells = grid_.getTotalCells();
        if (totalCells > 0)
        {
            stats_.percentBurned = (static_cast<float>(stats_.totalCellsBurnedOut) / totalCells) * 100.0f;
        }
    }

    void Simulation::detectCellChanges()
    {
        size_t idx = 0;

        grid_.forEach([this, &idx](const Cell &cell)
                      {
        CellState currentState = cell.getState();
        CellState previousState = previousStates_[idx];
        
        if (!isBurning(previousState) && isBurning(currentState)) {
            stats_.totalCellsIgnited++;
            for (auto* observer : observers_) {
                observer->onCellIgnited(cell.getX(), cell.getY());
            }
        }
        
        if (isBurning(previousState) && 
            (currentState == CellState::Burned || currentState == CellState::Supressed)) {
            for (auto* observer : observers_) {
                observer->onCellBurnedOut(cell.getX(), cell.getY());
            }
        }
        
        previousStates_[idx] = currentState;
        idx++; });
    }
}