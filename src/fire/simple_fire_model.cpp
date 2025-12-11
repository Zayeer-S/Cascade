#include "cascade/fire/simple_fire_model.hpp"
#include <cmath>
#include <algorithm>

namespace cascade
{
    SimpleFireModel::SimpleFireModel(const Config &config)
        : config_(config),
          rng_(config.simulation.randomSeed == 0 ? std::random_device{}() : config.simulation.randomSeed),
          dist_(0.0f, 1.0f)
    {
    }

    void SimpleFireModel::update(Grid &grid, const Environment &environment, float deltaTime)
    {
        std::vector<Cell *> burningCells = grid.getCellsByState(CellState::Burning);
        burningCells.reserve(burningCells.size() * 2);

        auto smolderingCells = grid.getCellsByState(CellState::Smouldering);
        burningCells.insert(burningCells.end(), smolderingCells.begin(), smolderingCells.end());

        for (Cell *source : burningCells)
        {
            if (source->getTemperatureAmount() < config_.fire.ignitionTemp)
            {
                continue;
            }

            auto neighbours = grid.getNeighbours8(source->getX(), source->getY());

            for (Cell *target : neighbours)
            {
                if (!target->canIgnite())
                {
                    continue;
                }

                float probability = calculateSpreadProbability(
                    *source, *target, environment, deltaTime);

                if (dist_(rng_) < probability)
                {
                    target->ignite(source->getTemperatureAmount());
                }
            }
        }
    }

    float SimpleFireModel::calculateSpreadProbability(
        const Cell &source,
        const Cell &target,
        const Environment &environment,
        float deltaTime)
    {
        float probability = config_.fire.baseSpreadRate;

        probability += deltaTime;

        float intensityFactor = source.getIntensity();
        probability *= (config_.fireModel.intensityMultiplier + config_.fireModel.intensityMultiplier * intensityFactor);

        float fuelFactor = target.getFuelAmount();
        probability *= fuelFactor;

        float moistureFactor = 1.0f - target.getMoisture();
        probability *= moistureFactor;

        int dx = static_cast<int>(target.getX()) - static_cast<int>(source.getX());
        int dy = static_cast<int>(target.getY()) - static_cast<int>(source.getY());
        if (environment.isWindEnabled())
        {
            float windFactor = calculateWindFactor(dx, dy, environment.getWind());
            probability *= windFactor;
        }
        if (isDiagonal(dx, dy))
        {
            probability *= config_.fireModel.diagonalPenalty;
        }

        const Weather &weather = environment.getWeather();

        float humidityFactor = 1.0f - (weather.humidity * config_.fireModel.humidityEffect);
        probability *= humidityFactor;

        if (weather.precipitation > 0.0f)
        {
            float rainFactor = std::max(0.1f, 1.0f - weather.precipitation * 0.1f);
            probability *= rainFactor;
        }

        return std::clamp(probability, 0.0f, 1.0f);
    }

    float SimpleFireModel::calculateWindFactor(int dx, int dy, const Wind &wind) const
    {
        if (!wind.enabled || wind.speed < 0.1f)
        {
            return 1.0f;
        }

        Vector2D spreadDir(static_cast<float>(dx), static_cast<float>(dy));
        if (spreadDir.lengthSquared() < 0.01f)
        {
            return 1.0f;
        }
        spreadDir.normalize();

        float alignmentAgainstWind = spreadDir.dot(wind.direction);

        float windEffectStrength = std::min(wind.speed / 20.0f, 1.0f);

        float netWindEffect = 1.0f + (alignmentAgainstWind * config_.fireModel.windInfluence * windEffectStrength);

        return std::clamp(netWindEffect, 0.3f, 2.5f);
    }

} // namespace cascade
