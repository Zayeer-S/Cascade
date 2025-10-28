#ifndef CASCADE_FIRE_SIMPLEFIREMODEL_HPP
#define CASCADE_FIRE_SIMPLEFIREMODEL_HPP

#include "fire_model.hpp"
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
        explicit SimpleFireModel(unsigned int seed = 0);

        void update(Grid &grid, const Environment &environment, float deltaTime) override;

        const char *getName() const override { return "SimpleFireModel"; }

        void reset() override {}

        void setBaseSpreadRate(float rate) { baseSpreadRate_ = rate; }
        float getBaseSpreadRate() const { return baseSpreadRate_; }

        void setWindInfluence(float rate) { windInfluence_ = rate; }
        float getWindInfluence() const { return windInfluence_; }

        void setDiagonalPenalty(float penalty) { diagonalPenalty_ = penalty; }
        float getDiagonalPenalty() const { return diagonalPenalty_; }

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

        float baseSpreadRate_;
        float windInfluence_;
        float diagonalPenalty_;
        float temperatureThreshold_;

        std::mt19937 rng_;
        std::uniform_real_distribution<float> dist_;
    };

} // namespace cascade

#endif