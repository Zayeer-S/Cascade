#include "cascade/grid/cell.hpp"
#include <algorithm>
#include <cmath>

namespace cascade
{
    namespace
    {
        constexpr float INITIAL_FUEL = 1.0f;
        constexpr float INTIIAL_MOISTURE = 0.3f;
        constexpr float AMBIENT_TEMP = 20.0f;
        constexpr float IGNITION_TEMP = 300.0f;
        constexpr float MAX_TEMP = 1000.0f;
        constexpr float BURN_RATE = 0.1f;           // Fuel consumed per second
        constexpr float COOLING_RATE = 10.0f;       // Temperature decrease per second
        constexpr float HEAT_TRANSFER = 50.0f;      // Heat increase when burning
        constexpr float SMOULDER_THRESHOLD = 0.3f;  // Fuel level to transition to smoldering
        constexpr float SMOULDER_BURN_RATE = 0.03f; // Slower burn rate when smoldering
        constexpr float WET_EVAPORATION = 0.05f;    // Moisture loss per second
    }

    Cell::Cell(size_t x, size_t y) : x_(x), y_(y), state_(CellState::Unburned), fuel_(INITIAL_FUEL), temperature_(AMBIENT_TEMP), moisture_(INTIIAL_MOISTURE), burnTime_(0.0f)
    {
    }

    void Cell::update(float deltaTime)
    {
        switch (state_)
        {
        case CellState::Burning:
            updateBurning(deltaTime);
            break;

        case CellState::Smouldering:
            updateSmouldering(deltaTime);
            break;

        case CellState::Burned:
        case CellState::Supressed:
            updateCooling(deltaTime);
            break;

        case CellState::Wet:
            updateWet(deltaTime);
            break;

        case CellState::Unburned:
        case CellState::Igniting:
            // No auto updates for these states
            break;
        }
    }

    void Cell::updateBurning(float deltaTime)
    {
        burnTime_ += deltaTime;

        fuel_ -= BURN_RATE * deltaTime;

        temperature_ = std::min(MAX_TEMP, temperature_ + HEAT_TRANSFER * deltaTime);

        moisture_ = std::max(0.0f, moisture_ - 0.1f * deltaTime);

        if (fuel_ <= 0.0f)
        {
            transitionToBurned();
        }
        else if (fuel_ < SMOULDER_THRESHOLD)
        {
            transitionToSmouldering();
        }
    }

    void Cell::updateSmouldering(float deltaTime)
    {
        burnTime_ += deltaTime;

        fuel_ -= SMOULDER_BURN_RATE * deltaTime;

        temperature_ = std::max(IGNITION_TEMP, temperature_ - COOLING_RATE * deltaTime * 0.5f);

        if (fuel_ <= 0.0f)
        {
            transitionToBurned();
        }
    }

    void Cell::updateCooling(float deltaTime) {
        if (temperature_ > AMBIENT_TEMP) {
            temperature_ = std::max(AMBIENT_TEMP, temperature_ - COOLING_RATE * deltaTime);
        }
    }

    void Cell::updateWet(float deltaTime) {
        moisture_ -= WET_EVAPORATION * deltaTime;

        if (moisture_ <= INTIIAL_MOISTURE) {
            state_ = CellState::Unburned;
            moisture_ = INTIIAL_MOISTURE;
        }

        updateCooling(deltaTime);
    }

    bool Cell::ignite(float intensity) {
        if (!canIgnite()) {
            return false;
        }

        if (moisture_ > 0.6f) {
            return false;
        }

        state_ = CellState::Burning;
        temperature_=std::max(temperature_, intensity);
        burnTime_ = 0.0f;

        return true;
    }

    bool Cell::extinguish(float waterAmount) {
        if (state_ == CellState::Burning || state_ == CellState::Smouldering) {
            temperature_ -= waterAmount * 100.0f;

            moisture_ = std::min(1.0f, moisture_ + waterAmount * 0.1f);

            if (temperature_ < IGNITION_TEMP) {
                state_ = CellState::Supressed;
                burnTime_ = 0.0f;
                return true;
            }
        }
    }

    float Cell::getIntensity() const {
        if (!isBurning()) {
            return 0.0f;
        }

        float normalized = (temperature_ - IGNITION_TEMP) / (MAX_TEMP - IGNITION_TEMP);

        return std::clamp(normalized, 0.0f, 1.0f);
    }

    void Cell::reset() {
        state_ = CellState::Unburned;
        fuel_ = INITIAL_FUEL;
        temperature_ = AMBIENT_TEMP;
        moisture_ = INTIIAL_MOISTURE;
        burnTime_ = 0.0f;
    }
    
    void Cell::transitionToSmouldering() {
        state_ = CellState::Smouldering;
    }

    void Cell::transitionToBurned() {
        state_ = CellState::Burned;
        fuel_ = 0.0f;
        burnTime_ = 0.0f;
    }
} // namespace cascade
