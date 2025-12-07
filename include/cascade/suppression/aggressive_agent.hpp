#ifndef CASCADE_SUPPRESSION_AGGRESSIVEAGENT_HPP
#define CASCADE_SUPPRESSION_AGGRESSIVEAGENT_HPP

#include "suppression_agent.hpp"

namespace cascade
{
    /**
     * @brief Its in the name
     *
     * Attacks burning cells directly, prioritizing:
     * 1. Highest intensity fires
     * 2. Most fuel remaining
     * 3. Closest to agent position
     */
    class AggressiveAgent : public SuppressionAgent
    {
    public:
        /**
         * @brief Construct aggressive agent
         * @param name Agent name
         * @param waterCapacity Total water capacity
         * @param waterPerCell Water per application
         */
        AggressiveAgent(const std::string &name, float waterCapacity, float waterPerCell = 0.5f);

        /**
         * @brief Set agent position for proximity-based targeting
         * @param x X co-ordinate
         * @param y Y co-ordinate
         */
        void setPosition(float x, float y)
        {
            position_ = Vector2D(x, y);
            hasPosition_ = true;
        }

        /**
         * @brief Set maximum working radius
         * @param radius Maximum distance for suppression
         */
        void setMaxRadius(float radius) { maxRadius_ = radius; }

    protected:
        std::vector<Cell *> selectTargets(Grid &grid) override;

    private:
        /**
         * @brief Calculate priority score for a burning cell
         * @param cell Target cell
         * @return Priority score (higher = more important)
         */
        float calculatePriority(const Cell *cell) const;

        Vector2D position_;
        bool hasPosition_;
        float maxRadius_;
    };
}

#endif