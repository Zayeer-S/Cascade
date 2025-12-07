#ifndef CASCADE_SUPPRESSION_DEFENSIVEAGENT_HPP
#define CASCADE_SUPPRESSION_DEFENSIVEAGENT_HPP

#include "suppression_agent.hpp"

namespace cascade {

/**
 * @brief Defensive suppression strategy
 * 
 * Creates firebreaks by wetting unburned cells ahead of the fire, prioritizing:
 * 1. Unburned cells adjacent to burning cells
 * 2. Cells in the fire's path (considering wind)
 * 3. High fuel cells that pose risk
 */
class DefensiveAgent : public SuppressionAgent {
public:
    /**
     * @brief Construct defensive agent
     * @param name Agent name
     * @param waterCapacity Total water capacity
     * @param waterPerCell Water per application
     */
    DefensiveAgent(const std::string& name, float waterCapacity, float waterPerCell = 0.5f);

    /**
     * @brief Set protected zone center and radius
     * @param x X coordinate of zone center
     * @param y Y coordinate of zone center
     * @param radius Protection radius
     */
    void setProtectedZone(float x, float y, float radius) {
        protectedZone_ = Vector2D(x, y);
        protectionRadius_ = radius;
        hasProtectedZone_ = true;
    }

    /**
     * @brief Set lookahead distance for firebreak placement
     * @param distance Number of cells to look ahead
     */
    void setLookahead(size_t distance) { lookahead_ = distance; }

protected:
    std::vector<Cell*> selectTargets(Grid& grid) override;

private:
    /**
     * @brief Calculate priority for firebreak placement
     * @param cell Target cell
     * @param grid Grid reference
     * @return Priority score
     */
    float calculateFirebreakPriority(const Cell* cell, const Grid& grid) const;

    /**
     * @brief Count burning neighbors
     * @param cell Target cell
     * @param grid Grid reference
     * @return Number of burning neighbors
     */
    size_t countBurningNeighbours(const Cell* cell, const Grid& grid) const;

    Vector2D protectedZone_;
    float protectionRadius_;
    bool hasProtectedZone_;
    size_t lookahead_;
};

} // namespace cascade

#endif