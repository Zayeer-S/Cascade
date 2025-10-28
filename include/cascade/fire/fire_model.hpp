#ifndef CASCADE_FIRE_FIREMODEL_HPP
#define CASCADE_FIRE_FIREMODEL_HPP

#include "cascade/grid/grid.hpp"
#include "cascade/environment/environment.hpp"
#include <random>

namespace cascade
{
    /**
     * @brief Abstract base class for fire spread models
     *
     * FireModel defines the interface for different fire simulation algorithms
     * Implementations can provide simple probabilistic spread, complex physics-based models, or anythin in between.
     */

    class FireModel
    {
    public:
        virtual ~FireModel() = default;

        /**
         * @brief Update fire spread across the grid
         * @param grid The simulation grid to update
         * @param environment Environmental conditions (wind, weather, etc)
         * @param deltaTime Time elapsed since last update (seconds)
         */
        virtual void update(Grid &grid, const Environment &environment, float deltaTime) = 0;

        /**
         * @brief Reset internal state
         */
        virtual void reset();

        virtual const char *getName() const = 0;

    protected:
        /**
         * @brief Calculate spread probability from source to target cell
         * @param source Cell that is currently burning
         * @param target Cell that might ignite
         * @param environment Current environmental conditions
         * @param deltaTime Time step for probability calculation
         * @return Probabiltiy of spread (0.0 - 1.0)
         */
        virtual float calculateSpreadProbability(
            const Cell &source,
            const Cell &target,
            const Environment &environment,
            float deltaTime) = 0;
    };

} // namespace cascade

#endif