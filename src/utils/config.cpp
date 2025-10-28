#include "cascade/utils/config.hpp"
#include "cascade/utils/validation.hpp"
#include <iostream>
#include <fstream>

namespace cascade
{
    Config Config::loadFromFile(const std::string &path)
    {
        // TODO MVP: Not implemented - return default config
        std::cerr << "Warning: Config file loading not implemented. Using defaults instead of: " << path;
        return Config::createDefault();
    }

    bool Config::saveToFile(const std::string &path) const
    {
        // TODO MVP: Not implemented fpr coursework
        std::cerr << "Warning: Config file saving not implemented. Not saving to: " << path;
        return false;
    }

    ValidationResult Config::validate() const
    {
        ValidationResult result;

        if (grid.width == 0 || grid.height == 0)
        {
            result.addError("Grid dimensions must be greater than 0");
        }

        if (grid.width > 1000 || grid.height > 1000)
        {
            result.addWarning("Grid dimensions very large (" +
                              std::to_string(grid.width) + "x" +
                              std::to_string(grid.height) + ").");
        }

        if (fire.baseSpreadRate < 0.0f || fire.baseSpreadRate > 1.0f)
        {
            result.addError("Base spread rate must be in range [0.0, 1.0]");
        }

        if (fire.ignitionTemp <= fire.ambientTemp)
        {
            result.addError("Ignition temperature must be greater than ambient temperature");
        }

        if (fire.burnRate <= 0.0f)
        {
            result.addError("Burn rate must be positive");
        }

        if (fire.heatTransfer < 0.0f)
        {
            result.addError("Heat transfer cannot be negative");
        }

        if (fire.coolingRate < 0.0f)
        {
            result.addError("Cooling rate cannot be negative");
        }

        if (wind.speed < 0.0f) {
            result.addError("Wind speed cannot be negative");
        }

        if (suppression.defaultRange <= 0.0f) {
            result.addError("Suppresion must be positive");
        }

        if (suppression.defaultFlowRate <= 0.0f) {
            result.addError("Flow rate must be positive");
        }

        if (suppression.waterEffectiveness <= 0.0f) {
            result.addError("Water effectiveness must be postive");
        }

        if (simulation.timeStep <= 0.0f || simulation.timeStep > 1.0f) {
            result.addError("Time step must be in range (0.0, 1.0)");
        }

        return result;
    }
}