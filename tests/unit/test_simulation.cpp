#include "cascade/core/simulation.hpp"
#include "cascade/fire/simple_fire_model.hpp"
#include <gtest/gtest.h>

using namespace cascade;

class TestObserver : public SimulationObserver {
public:
    int startCalls = 0;
    int stepCalls = 0;
    int endCalls = 0;
    int ignitedCalls = 0;
    int burnedOutCalls = 0;
    std::string endReason;

    void onSimulationStart(const Simulation&) override {
        startCalls++;
    }

    void onSimulationStep(const Simulation&, size_t) override {
        stepCalls++;
    }

    void onSimulationEnd(const Simulation&, const char* reason) override {
        endCalls++;
        endReason = reason;
    }

    void onCellIgnited(size_t, size_t) override {
        ignitedCalls++;
    }

    void onCellBurnedOut(size_t, size_t) override {
        burnedOutCalls++;
    }

    void reset() {
        startCalls = 0;
        stepCalls = 0;
        endCalls = 0;
        ignitedCalls = 0;
        burnedOutCalls = 0;
        endReason.clear();
    }
};

class SimulationTest : public ::testing::Test {
protected:
    void SetUp() override {
        sim = new Simulation(20, 20);
        observer = new TestObserver();
    }

    void TearDown() override {
        delete observer;
        delete sim;
    }

    Simulation* sim;
    TestObserver* observer;
};

TEST_F(SimulationTest, Construction) {
    EXPECT_EQ(sim->getGrid().getWidth(), 20);
    EXPECT_EQ(sim->getGrid().getHeight(), 20);
    EXPECT_FALSE(sim->isRunning());
    EXPECT_FALSE(sim->isFireActive());
}

TEST_F(SimulationTest, InitializationRequired) {
    EXPECT_FALSE(sim->step());
}

TEST_F(SimulationTest, InitializeWithFireModel) {
    auto fireModel = std::make_unique<SimpleFireModel>(12345);
    sim->initialize(std::move(fireModel));
    
    EXPECT_NE(sim->getFireModel(), nullptr);
    EXPECT_EQ(sim->getStepCount(), 0);
    EXPECT_FLOAT_EQ(sim->getSimulationTime(), 0.0f);
}

TEST_F(SimulationTest, InitializeNullFireModelThrows) {
    EXPECT_THROW(sim->initialize(nullptr), std::invalid_argument);
}

TEST_F(SimulationTest, SingleStep) {
    sim->initialize(std::make_unique<SimpleFireModel>(12345));
    sim->getGrid().getCell(10, 10).ignite();
    
    EXPECT_TRUE(sim->step(0.1f));
    EXPECT_EQ(sim->getStepCount(), 1);
    EXPECT_FLOAT_EQ(sim->getSimulationTime(), 0.1f);
}

TEST_F(SimulationTest, MultipleSteps) {
    sim->initialize(std::make_unique<SimpleFireModel>(12345));
    sim->getGrid().getCell(10, 10).ignite();
    
    for (int i = 0; i < 5; ++i) {
        sim->step(0.1f);
    }
    
    EXPECT_EQ(sim->getStepCount(), 5);
    EXPECT_NEAR(sim->getSimulationTime(), 0.5f, 0.001f);
}

TEST_F(SimulationTest, StepReturnsFalseWhenFireOut) {
    sim->initialize(std::make_unique<SimpleFireModel>(12345));
    
    EXPECT_FALSE(sim->step());
    EXPECT_FALSE(sim->isFireActive());
}

TEST_F(SimulationTest, InitialStatistics) {
    sim->initialize(std::make_unique<SimpleFireModel>(12345));
    
    const auto& stats = sim->getStats();
    EXPECT_EQ(stats.unburnedCells, 400);
    EXPECT_EQ(stats.burningCells, 0);
    EXPECT_EQ(stats.burnedCells, 0);
    EXPECT_EQ(stats.stepCount, 0);
    EXPECT_FLOAT_EQ(stats.simulationTime, 0.0f);
}

TEST_F(SimulationTest, StatisticsUpdate) {
    sim->initialize(std::make_unique<SimpleFireModel>(12345));
    sim->getGrid().getCell(10, 10).ignite();
    
    sim->step(0.1f);
    
    const auto& stats = sim->getStats();
    EXPECT_GT(stats.burningCells, 0);
    EXPECT_LT(stats.unburnedCells, 400);
    EXPECT_TRUE(stats.isFireActive());
}

TEST_F(SimulationTest, TotalCellsIgnitedTracked) {
    sim->initialize(std::make_unique<SimpleFireModel>(12345));
    
    sim->getGrid().getCell(5, 5).ignite();
    sim->getGrid().getCell(15, 15).ignite();
    
    for (int i = 0; i < 10; ++i) {
        if (!sim->step(0.1f)) break;
    }
    
    const auto& stats = sim->getStats();
    EXPECT_GE(stats.totalCellsIgnited, 2);
}

TEST_F(SimulationTest, ObserverNotifiedOnStart) {
    sim->addObserver(observer);
    sim->initialize(std::make_unique<SimpleFireModel>(12345));
    
    EXPECT_EQ(observer->startCalls, 1);
}

TEST_F(SimulationTest, ObserverNotifiedOnStep) {
    sim->initialize(std::make_unique<SimpleFireModel>(12345));
    sim->addObserver(observer);
    
    sim->getGrid().getCell(10, 10).ignite();
    
    sim->step(0.1f);
    EXPECT_EQ(observer->stepCalls, 1);
    
    sim->step(0.1f);
    EXPECT_EQ(observer->stepCalls, 2);
}

TEST_F(SimulationTest, ObserverNotifiedOnIgnition) {
    sim->initialize(std::make_unique<SimpleFireModel>(12345));
    sim->addObserver(observer);
    
    static_cast<SimpleFireModel*>(sim->getFireModel())->setBaseSpreadRate(1.0f);
    
    sim->getGrid().getCell(10, 10).ignite();
    
    EXPECT_EQ(observer->ignitedCalls, 0);
    
    for (int i = 0; i < 10; ++i) {
        sim->step(0.1f);
    }
    
    EXPECT_GT(observer->ignitedCalls, 0);
}

TEST_F(SimulationTest, ObserverNotifiedOnEnd) {
    sim->initialize(std::make_unique<SimpleFireModel>(12345));
    sim->addObserver(observer);
    
    sim->step();
    
    EXPECT_EQ(observer->endCalls, 1);
    EXPECT_EQ(observer->endReason, "burned_out");
}

TEST_F(SimulationTest, RemoveObserver) {
    sim->initialize(std::make_unique<SimpleFireModel>(12345));
    sim->addObserver(observer);
    sim->getGrid().getCell(10, 10).ignite();
    
    sim->step();
    EXPECT_EQ(observer->stepCalls, 1);
    
    sim->removeObserver(observer);
    sim->step();
    EXPECT_EQ(observer->stepCalls, 1);
}

TEST_F(SimulationTest, RunForDuration) {
    sim->initialize(std::make_unique<SimpleFireModel>(12345));
    sim->getGrid().getCell(10, 10).ignite();
    
    size_t steps = sim->run(1.0f, 0.1f);
    
    EXPECT_EQ(steps, 10);
    EXPECT_NEAR(sim->getSimulationTime(), 1.0f, 0.001f);
}

TEST_F(SimulationTest, RunUntilComplete) {
    sim->initialize(std::make_unique<SimpleFireModel>(12345));
    
    sim->getGrid().getCell(10, 10).ignite();
    
    size_t steps = sim->runUntilComplete(0, 0.1f);
    
    EXPECT_GT(steps, 0);
    EXPECT_FALSE(sim->isFireActive());
}

TEST_F(SimulationTest, RunUntilMaxSteps) {
    sim->initialize(std::make_unique<SimpleFireModel>(12345));
    sim->addObserver(observer);
    
    for (size_t x = 8; x < 13; ++x) {
        for (size_t y = 8; y < 13; ++y) {
            sim->getGrid().getCell(x, y).ignite();
        }
    }
    
    static_cast<SimpleFireModel*>(sim->getFireModel())->setBaseSpreadRate(0.5f);
    
    size_t steps = sim->runUntilComplete(20, 0.1f);
    
    EXPECT_EQ(steps, 20);
    EXPECT_EQ(observer->endReason, "max_steps_reached");
}

TEST_F(SimulationTest, Reset) {
    sim->initialize(std::make_unique<SimpleFireModel>(12345));
    sim->getGrid().getCell(10, 10).ignite();
    
    sim->run(0.5f, 0.1f);
    
    EXPECT_GT(sim->getStepCount(), 0);
    EXPECT_GT(sim->getSimulationTime(), 0.0f);
    
    sim->reset();
    
    EXPECT_EQ(sim->getStepCount(), 0);
    EXPECT_FLOAT_EQ(sim->getSimulationTime(), 0.0f);
    EXPECT_FALSE(sim->isFireActive());
}

TEST_F(SimulationTest, CompleteSimulationWithObserver) {
    sim->initialize(std::make_unique<SimpleFireModel>(12345));
    sim->addObserver(observer);
    
    sim->getGrid().getCell(10, 10).ignite();
    
    sim->runUntilComplete(100, 0.1f);
    
    EXPECT_EQ(observer->startCalls, 1);
    EXPECT_GT(observer->stepCalls, 0);
    EXPECT_EQ(observer->endCalls, 1);
    EXPECT_GT(observer->ignitedCalls, 0);
}

TEST_F(SimulationTest, LargeGridPerformance) {
    Simulation largeSim(100, 100);
    largeSim.initialize(std::make_unique<SimpleFireModel>(12345));
    
    largeSim.getGrid().getCell(25, 25).ignite();
    largeSim.getGrid().getCell(75, 75).ignite();
    
    size_t steps = largeSim.runUntilComplete(50, 0.1f);
    
    EXPECT_GT(steps, 0);
}

TEST_F(SimulationTest, StopSimulation) {
    sim->initialize(std::make_unique<SimpleFireModel>(12345));
    
    for (size_t x = 5; x < 15; ++x) {
        for (size_t y = 5; y < 15; ++y) {
            sim->getGrid().getCell(x, y).ignite();
        }
    }
    
    sim->step();
    sim->step();
    sim->stop();
    
    EXPECT_FALSE(sim->isRunning());
}

TEST_F(SimulationTest, AverageStepTimeTracked) {
    sim->initialize(std::make_unique<SimpleFireModel>(12345));
    sim->getGrid().getCell(10, 10).ignite();
    
    for (int i = 0; i < 5; ++i) {
        sim->step(0.1f);
    }
    
    const auto& stats = sim->getStats();
    EXPECT_GT(stats.averageStepTime, 0.0f);
}