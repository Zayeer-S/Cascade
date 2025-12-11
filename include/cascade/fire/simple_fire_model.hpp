#ifndef CASCADE_FIRE_SIMPLEFIREMODEL_HPP
#define CASCADE_FIRE_SIMPLEFIREMODEL_HPP

#include "fire_model.hpp"
#include "cascade/utils/config.hpp"
#include <random>

namespace cascade
{

    class SimpleFireModel : public FireModel
    {
    public:
        /**
         * @brief Simple probalistic fire spread model
         *
         * Uses following rules for fire spreading:
         *
         */
        explicit SimpleFireModel(const Config& config);

        void update(Grid &grid, const Environment &environment, float deltaTime) override;

        const char *getName() const override { return "SimpleFireModel"; }

        void reset() override {}

        float getBaseSpreadRate() const { return config_.fire.baseSpreadRate; }
        float getWindInfluence() const { return config_.fireModel.windInfluence; }
        float getDiagonalPenalty() const { return config_.fireModel.diagonalPenalty; }

    protected:
        float calculateSpreadProbability(
            const Cell &source,
            const Cell &target,
            const Environment &environment,
            float deltaTime) override;

    private:
        /**
         * @brief Calculate wind factor for spread direction
         * @param dx X offset from source to target
         * @param dy Y offset from source to target
         * @param wind Current wind conditions
         * @return Wind influence multiplier (0.5 - 2.0)
         */
        float calculateWindFactor(int dx, int dy, const Wind &wind) const;

        /**
         * @brief Check if diagonal neighbour
         * @param dx X offset
         * @param dy Y offset
         * @return true if diagonal
         */
        bool isDiagonal(int dx, int dy) const
        {
            return dx != 0 && dy != 0;
        }

        const Config& config_;
        std::mt19937 rng_;
        std::uniform_real_distribution<float> dist_;
    };

} // namespace cascade

#endif