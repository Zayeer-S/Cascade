#ifndef CASCADE_UTILS_AGENT_CONFIG_HPP
#define CASCADE_UTILS_AGENT_CONFIG_HPP

#include "cascade/utils/vector2d.hpp"
#include "cascade/utils/config.hpp"
#include <vector>
#include <string>

namespace cascade
{
    /**
     * @brief Configuration for agent spawning in simulation
     */
    struct AgentSpawnConfig
    {
        enum class SpawnMode
        {
            Random, ///< Random position on grid
            Custom, ///< User-specified position
            Center  ///< Centre of grid
        };

        enum class AgentType
        {
            Aggressive, ///< Attacks burning cells directly
            Defensive   ///< Creates firebreaks ahead of fire
        };

        /**
         * @brief Specification for a single agent
         */
        struct AgentSpec
        {
            AgentType type;
            std::string name;
            float waterCapacity;
            float waterPerCell;
            size_t spawnStep; ///< Simulation step to spawn (0 = immediately)
            SpawnMode spawnMode;
            Vector2D customPosition; ///< Used if spawnMode == Custom

            AgentSpec(const Config &config)
                : type(AgentType::Aggressive),
                  name("Agent"),
                  waterCapacity(config.agent.defaultWaterCapacity),
                  waterPerCell(config.agent.defaultWaterPerCell),
                  spawnStep(config.agent.defaultSpawnStep),
                  spawnMode(SpawnMode::Random),
                  customPosition(0.0f, 0.0f)
            {
            }
        };

        std::vector<AgentSpec> agents;

        /**
         * @brief Create default config (no agents)
         */
        static AgentSpawnConfig createDefault()
        {
            return AgentSpawnConfig();
        }

        /**
         * @brief Create config through interactive prompts
         * @param config Simulation config for validation
         * @return Configured agent spawn config
         */
        static AgentSpawnConfig createInteractive(const Config &config);

        /**
         * @brief Add an agent specification
         */
        void addAgent(const AgentSpec &spec)
        {
            agents.push_back(spec);
        }

        /**
         * @brief Get number of agents
         */
        size_t getAgentCount() const
        {
            return agents.size();
        }

        /**
         * @brief Check if any agents are configured
         */
        bool hasAgents() const
        {
            return !agents.empty();
        }
    };

} // namespace cascade

#endif