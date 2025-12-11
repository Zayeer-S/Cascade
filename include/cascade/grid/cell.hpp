#ifndef CASCADE_GRID_CELL_HPP
#define CASCADE_GRID_CELL_HPP

#include "cell_state.hpp"
#include "cascade/utils/vector2d.hpp"
#include "cascade/utils/config.hpp"

namespace cascade
{
    /**
     * @brief Represents a single cell in the simulation grid
     *
     * A cell maintains its position, state, and physical propertites (e.g. fuel, temp, moisture content)
     */

    class Cell
    {
    public:
        /**
         * @brief Construct a new cell
         * @param x X coordinate in grid
         * @param y Y coordinate in grid
         * @param config Configuration reference for cell behavior
         */
        Cell(size_t x, size_t y, const Config& config);

        /**
         * @brief Update cell state based on elapsed time
         * @param deltaTime Time elapsed since last update (seconds)
         */
        void update(float deltaTime);

        /**
         * @brief Attempt to ignite this cell
         * @param intensity Initial fire intensity (celsius)
         * @return true if ignition successful, false otherwise
         */
        bool ignite(float intensity = 300.0f);

        /**
         * @brief Apply water supression to this cell
         * @param waterAmount Amount of water applied
         * @return true if cell was affected, false otherise
         */
        bool extinguish(float waterAmount);

        // All getters
        CellState getState() const { return state_; }
        float getFuelAmount() const { return fuel_; }
        float getTemperatureAmount() const { return temperature_; }
        float getMoisture() const { return moisture_; }
        float getBurnTime() const { return burnTime_; }
        size_t getX() const { return x_; }
        size_t getY() const { return y_; }
        Vector2D getPosition() const { return Vector2D(static_cast<float>(x_), static_cast<float>(y_)); }

        /**
         * @brief Get fire intensity (0.0 to 1.0)
         * @return Normalized intensity based on temperature
         */
        float getIntensity() const;

        /**
         * @brief Check if cell is currently burning
         * @return true if in any burning state
         */
        bool isBurning() const { return cascade::isBurning(state_); }

        /**
         * @brief Check if cell can currently ignite
         * @return true if cell can catch fire
         */
        bool canIgnite() const { return cascade::canIgnite(state_) && fuel_ > 0.0f; }

        // Setters
        void setState(CellState state) { state_ = state; }
        void setFuel(float fuel) { fuel_ = fuel; }
        void setTemperature(float temperature) { temperature_ = temperature; }
        void setMoisture(float moisture) { moisture_ = moisture; }

        /**
         * @brief Reset call to unburned/initial state
         */
        void reset();

    private:
        size_t x_;
        size_t y_;

        CellState state_;
        float fuel_;
        float temperature_;
        float moisture_;
        float burnTime_;

        const Config& config_;

        void updateBurning(float deltaTime);
        void updateSmouldering(float deltaTime);
        void updateCooling(float deltaTime);
        void updateWet(float deltaTime);

        void transitionToSmouldering();
        void transitionToBurned();
    };
}

#endif