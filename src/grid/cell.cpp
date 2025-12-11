#include "cascade/grid/cell.hpp"
#include <algorithm>
#include <cmath>

namespace cascade
{
    Cell::Cell(size_t x, size_t y, const Config &config)
        : x_(x),
          y_(y),
          state_(CellState::Unburned),
          fuel_(config.cell.initialFuel),
          temperature_(config.fire.ambientTemp),
          moisture_(config.cell.initialMoisture),
          burnTime_(0.0f),
          config_(config)
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
        case CellState::Suppressed:
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

        fuel_ -= config_.fire.burnRate * deltaTime;

        temperature_ = std::min(config_.fire.maxIntensity,
                                temperature_ + config_.fire.heatTransfer * deltaTime);

        moisture_ = std::max(0.0f, moisture_ - 0.1f * deltaTime);

        if (fuel_ <= 0.0f)
        {
            transitionToBurned();
        }
        else if (fuel_ < config_.cell.smoulderThreshold)
        {
            transitionToSmouldering();
        }
    }

    void Cell::updateSmouldering(float deltaTime)
    {
        burnTime_ += deltaTime;

        fuel_ -= config_.cell.smoulderBurnRate * deltaTime;

        temperature_ = std::max(config_.fire.ignitionTemp,
                                temperature_ - config_.fire.coolingRate * deltaTime * 0.5f);

        if (fuel_ <= 0.0f)
        {
            transitionToBurned();
        }
    }

    void Cell::updateCooling(float deltaTime)
    {
        if (temperature_ > config_.fire.ambientTemp)
        {
            temperature_ = std::max(config_.fire.ambientTemp,
                                    temperature_ - config_.fire.coolingRate * deltaTime);
        }
    }

    void Cell::updateWet(float deltaTime)
    {
        moisture_ -= config_.cell.wetEvaporationRate * deltaTime;

        if (moisture_ <= config_.cell.initialMoisture)
        {
            state_ = CellState::Unburned;
            moisture_ = config_.cell.initialMoisture;
        }

        updateCooling(deltaTime);
    }

    bool Cell::ignite(float intensity)
    {
        if (!canIgnite())
        {
            return false;
        }

        if (moisture_ > 0.6f)
        {
            return false;
        }

        state_ = CellState::Burning;
        temperature_ = std::max(temperature_, intensity);
        burnTime_ = 0.0f;

        return true;
    }

    bool Cell::extinguish(float waterAmount)
    {
        if (state_ == CellState::Burning || state_ == CellState::Smouldering)
        {
            temperature_ -= waterAmount * config_.suppression.waterEffectiveness;

            // Add moisture
            moisture_ = std::min(1.0f, moisture_ + waterAmount * config_.cell.moistureGainPerWater);

            if (temperature_ < config_.fire.ignitionTemp)
            {
                state_ = CellState::Suppressed;
                burnTime_ = 0.0f;
                return true;
            }
        }
        else if (state_ == CellState::Unburned)
        {
            state_ = CellState::Wet;
            moisture_ = std::min(1.0f, moisture_ + waterAmount * config_.cell.moistureGainPerWater);
            return true;
        }

        return false;
    }

    float Cell::getIntensity() const
    {
        if (!isBurning())
        {
            return 0.0f;
        }

        float normalized = (temperature_ - config_.fire.ignitionTemp) /
                           (config_.fire.maxIntensity - config_.fire.ignitionTemp);

        return std::clamp(normalized, 0.0f, 1.0f);
    }

    void Cell::reset()
    {
        state_ = CellState::Unburned;
        fuel_ = config_.cell.initialFuel;
        temperature_ = config_.fire.ambientTemp;
        moisture_ = config_.cell.initialMoisture;
        burnTime_ = 0.0f;
    }

    void Cell::transitionToSmouldering()
    {
        state_ = CellState::Smouldering;
    }

    void Cell::transitionToBurned()
    {
        state_ = CellState::Burned;
        fuel_ = 0.0f;
        burnTime_ = 0.0f;
    }
} // namespace cascade
