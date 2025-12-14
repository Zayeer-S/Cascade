#include "cascade/suppression/agent_factory.hpp"
#include <iostream>

namespace cascade
{
    std::mt19937 &AgentFactory::getRNG()
    {
        static std::random_device rd;
        static std::mt19937 rng(rd());
        return rng;
    }

    std::unique_ptr<SuppressionAgent> AgentFactory::createAgent(
        const AgentSpawnConfig::AgentSpec &spec)
    {
        std::unique_ptr<SuppressionAgent> agent;

        switch (spec.type)
        {
        case AgentSpawnConfig::AgentType::Aggressive:
            agent = std::make_unique<AggressiveAgent>(
                spec.name,
                spec.waterCapacity,
                spec.waterPerCell);
            break;

        case AgentSpawnConfig::AgentType::Defensive:
            agent = std::make_unique<DefensiveAgent>(
                spec.name,
                spec.waterCapacity,
                spec.waterPerCell);
            break;

        default:
            agent = std::make_unique<AggressiveAgent>(
                spec.name,
                spec.waterCapacity,
                spec.waterPerCell);
            break;
        }

        return agent;
    }

    void AgentFactory::setAgentPosition(
        SuppressionAgent *agent,
        const AgentSpawnConfig::AgentSpec &spec,
        const Grid &grid)
    {
        if (!agent)
        {
            return;
        }

        Vector2D position;

        switch (spec.spawnMode)
        {
        case AgentSpawnConfig::SpawnMode::Random:
            position = getRandomPosition(grid);
            break;

        case AgentSpawnConfig::SpawnMode::Custom:
        case AgentSpawnConfig::SpawnMode::Center:
            position = spec.customPosition;
            break;
        }

        if (auto *aggressive = dynamic_cast<AggressiveAgent *>(agent))
            aggressive->setPosition(position.x, position.y);

        else if (auto *defensive = dynamic_cast<DefensiveAgent *>(agent))
            defensive->setProtectedZone(position.x, position.y, 10.0f);

        std::cout << "\tPositioned " << agent->getName()
                  << "\t at(" << static_cast<int>(position.x)
                  << ", " << static_cast<int>(position.y) << ")";
    }

    Vector2D AgentFactory::getRandomPosition(const Grid &grid)
    {
        std::uniform_real_distribution<float> distX(0.0f, static_cast<float>(grid.getWidth() - 1));
        std::uniform_real_distribution<float> distY(0.0f, static_cast<float>(grid.getHeight() - 1));

        return Vector2D(distX(getRNG()), distY(getRNG()));
    }
    
} // namespace cascade
