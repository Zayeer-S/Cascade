#include "cascade/core/simulation.hpp"
#include "cascade/fire/simple_fire_model.hpp"
#include <gtest/gtest.h>

using namespace cascade;

/**
 * Integration tests for complete fire spread scenarios
 * Tests multi-component interaction and realistic use cases
 */

class FireSpreadIntegrationTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        sim = new Simulation(50, 50);
        auto fireModel = std::make_unique<SimpleFireModel>(42);
        fireModel->setBaseSpreadRate(0.4f);
        sim->initialize(std::move(fireModel));
    }

    void TearDown() override
    {
        delete sim;
    }

    Simulation *sim;
};

// Scenario: Single ignition point spreads naturally
TEST_F(FireSpreadIntegrationTest, SingleIgnitionSpread)
{
    sim->getGrid().getCell(25, 25).ignite();

    size_t initialBurning = sim->getStats().burningCells;

    for (int i = 0; i < 20; ++i)
    {
        if (!sim->step(0.1f))
            break;
    }

    size_t totalAffected = sim->getStats().burnedCells +
                           sim->getStats().burningCells;

    EXPECT_GT(totalAffected, initialBurning);
}

// Scenario: Multiple ignition points
TEST_F(FireSpreadIntegrationTest, MultipleIgnitionPoints)
{
    sim->getGrid().getCell(10, 10).ignite();
    sim->getGrid().getCell(40, 40).ignite();
    sim->getGrid().getCell(25, 10).ignite();

    sim->run(2.0f, 0.1f);

    size_t totalAffected = sim->getStats().burnedCells +
                           sim->getStats().burningCells;

    EXPECT_GT(totalAffected, 10);
}

// Scenario: Fire burns out completely
TEST_F(FireSpreadIntegrationTest, FireBurnsOut)
{
    sim->getGrid().getCell(25, 25).ignite();

    sim->runUntilComplete(0, 0.1f);

    EXPECT_FALSE(sim->isFireActive());
    EXPECT_EQ(sim->getStats().burningCells, 0);
    EXPECT_GT(sim->getStats().burnedCells, 0);
}

// Scenario: Fire spread is deterministic with same seed
TEST_F(FireSpreadIntegrationTest, DeterministicSpread)
{
    Simulation sim1(30, 30);
    auto model1 = std::make_unique<SimpleFireModel>(12345);
    model1->setBaseSpreadRate(0.4f);
    sim1.initialize(std::move(model1));
    sim1.getGrid().getCell(15, 15).ignite();
    sim1.run(1.0f, 0.1f);

    size_t affected1 = sim1.getStats().burnedCells + sim1.getStats().burningCells;

    Simulation sim2(30, 30);
    auto model2 = std::make_unique<SimpleFireModel>(12345);
    model2->setBaseSpreadRate(0.4f);
    sim2.initialize(std::move(model2));
    sim2.getGrid().getCell(15, 15).ignite();
    sim2.run(1.0f, 0.1f);

    size_t affected2 = sim2.getStats().burnedCells + sim2.getStats().burningCells;

    EXPECT_EQ(affected1, affected2);
}

// Scenario: Large fire on big grid
TEST_F(FireSpreadIntegrationTest, LargeScaleFire)
{
    Simulation largeSim(100, 100);
    auto fireModel = std::make_unique<SimpleFireModel>(999);
    fireModel->setBaseSpreadRate(0.5f);
    largeSim.initialize(std::move(fireModel));

    for (size_t x = 45; x < 55; ++x)
    {
        for (size_t y = 45; y < 55; ++y)
        {
            largeSim.getGrid().getCell(x, y).ignite();
        }
    }

    largeSim.run(2.0f, 0.1f);

    size_t affected = largeSim.getStats().burnedCells +
                      largeSim.getStats().burningCells;

    EXPECT_GT(affected, 100);
}

// Scenario: Fire spread rate affects coverage
TEST_F(FireSpreadIntegrationTest, SpreadRateImpact)
{
    Simulation slowSim(30, 30);
    auto slowModel = std::make_unique<SimpleFireModel>(111);
    slowModel->setBaseSpreadRate(0.2f);
    slowSim.initialize(std::move(slowModel));
    slowSim.getGrid().getCell(15, 15).ignite();
    slowSim.run(1.0f, 0.1f);

    size_t slowAffected = slowSim.getStats().burnedCells +
                          slowSim.getStats().burningCells;

    Simulation fastSim(30, 30);
    auto fastModel = std::make_unique<SimpleFireModel>(111);
    fastModel->setBaseSpreadRate(0.7f);
    fastSim.initialize(std::move(fastModel));
    fastSim.getGrid().getCell(15, 15).ignite();
    fastSim.run(1.0f, 0.1f);

    size_t fastAffected = fastSim.getStats().burnedCells +
                          fastSim.getStats().burningCells;

    EXPECT_GT(fastAffected, slowAffected);
}

// Scenario: Fire contained by wet cells
TEST_F(FireSpreadIntegrationTest, WetCellsContainFire)
{
    sim->getGrid().getCell(25, 25).ignite();

    for (size_t x = 23; x <= 27; ++x)
    {
        sim->getGrid().getCell(x, 23).extinguish(2.0f);
        sim->getGrid().getCell(x, 27).extinguish(2.0f);
    }
    for (size_t y = 24; y <= 26; ++y)
    {
        sim->getGrid().getCell(23, y).extinguish(2.0f);
        sim->getGrid().getCell(27, y).extinguish(2.0f);
    }

    sim->run(2.0f, 0.1f);

    size_t affected = sim->getStats().burnedCells +
                      sim->getStats().burningCells;

    EXPECT_LT(affected, 100); // Shouldn't spread too far
}