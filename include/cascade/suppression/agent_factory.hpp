#ifndef CASCADE_SUPPRESSION_AGENT_FACTORY_HPP
#define CASCADE_SUPPRESSION_AGENT_FACTORY_HPP

#include "cascade/suppression/suppression_agent.hpp"
#include "cascade/suppression/aggressive_agent.hpp"
#include "cascade/suppression/defensive_agent.hpp"
#include "cascade/utils/agent_config.hpp"
#include "cascade/grid/grid.hpp"
#include <memory>
#include <random>

namespace cascade
{
    /**
     * @brief Factory for creating and positioning supression agents
     */
    class AgentFactory
    {
    public:
        /**
         * @brief Create agent from specification
         * @param spec Agent specification
         * @return Unique pointer to created agent
         */
        static std::unique_ptr<SuppressionAgent> createAgent(
            const AgentSpawnConfig::AgentSpec &spec);

        /**
         * @brief Set agent position based on spawn node
         * @param agent Agent to position
         * @param spec Agent specification
         * @param grid Grid for position validation
         */
        static void setAgentPosition(
            SuppressionAgent *agent,
            const AgentSpawnConfig::AgentSpec &spec,
            const Grid &grid);

        /**
         * @brief Get random position on grid
         * @param grid Grid to get position from
         * @return Random position
         */
        static Vector2D getRandomPosition(const Grid &grid);

    private:
        static std::mt19937 &getRNG();
    };
} // namespace cascade

#endif