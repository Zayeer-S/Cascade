#include "cascade/fire/SimpleFireModel.hpp"
#include "cascade/environment/Environment.hpp"
#include <gtest/gtest.h>

using namespace cascade;

// Test fixture for FireModel tests
class FireModelTest : public ::testing::Test {
protected:
    void SetUp() override {
        grid = new Grid(20, 20);
        environment = new Environment();
        fireModel = new SimpleFireModel(12345); // Fixed seed for reproducibility
    }

    void TearDown() override {
        delete fireModel;
        delete environment;
        delete grid;
    }

    Grid* grid;
    Environment* environment;
    SimpleFireModel* fireModel;
};

// Basic Functionality Tests
TEST_F(FireModelTest, Construction) {
    EXPECT_STREQ(fireModel->getName(), "SimpleFireModel");
}

TEST_F(FireModelTest, ConfigurationAccessors) {
    fireModel->setBaseSpreadRate(0.5f);
    EXPECT_FLOAT_EQ(fireModel->getBaseSpreadRate(), 0.5f);

    fireModel->setWindInfluence(0.8f);
    EXPECT_FLOAT_EQ(fireModel->getWindInfluence(), 0.8f);

    fireModel->setDiagonalPenalty(0.6f);
    EXPECT_FLOAT_EQ(fireModel->getDiagonalPenalty(), 0.6f);
}

// Fire Spread Tests
TEST_F(FireModelTest, NoSpreadFromUnburned) {
    // No cells burning - nothing should happen
    fireModel->update(*grid, *environment, 0.1f);
    
    size_t burning = grid->countIf([](const Cell& c) { return c.isBurning(); });
    EXPECT_EQ(burning, 0);
}

TEST_F(FireModelTest, SpreadToNeighbors) {
    // Ignite center cell
    grid->getCell(10, 10).ignite();
    
    // Very high spread rate to ensure propagation
    fireModel->setBaseSpreadRate(2.0f);
    
    // Update multiple times
    for (int i = 0; i < 10; ++i) {
        fireModel->update(*grid, *environment, 0.1f);
    }
    
    // Fire should have spread to at least some neighbors
    size_t burning = grid->countIf([](const Cell& c) { return c.isBurning(); });
    EXPECT_GT(burning, 1);
}

TEST_F(FireModelTest, SpreadRespectsFuel) {
    // Create cell with no fuel
    grid->getCell(10, 10).ignite();
    grid->getCell(10, 11).setFuel(0.0f);
    
    fireModel->setBaseSpreadRate(2.0f);
    
    // Update many times
    for (int i = 0; i < 50; ++i) {
        fireModel->update(*grid, *environment, 0.1f);
    }
    
    // Cell with no fuel should not ignite
    EXPECT_FALSE(grid->getCell(10, 11).isBurning());
}

TEST_F(FireModelTest, SpreadRespectsMoisture) {
    // Create wet cell
    grid->getCell(10, 10).ignite();
    grid->getCell(10, 11).setMoisture(1.0f); // Completely wet
    
    fireModel->setBaseSpreadRate(2.0f);
    
    // Update many times
    for (int i = 0; i < 50; ++i) {
        fireModel->update(*grid, *environment, 0.1f);
    }
    
    // Wet cell should be very resistant (might eventually ignite but unlikely)
    // This is probabilistic, but with max moisture it should remain unburned
    EXPECT_FALSE(grid->getCell(10, 11).isBurning());
}

// Wind Effect Tests
TEST_F(FireModelTest, WindIncreasesDownwindSpread) {
    // Set wind blowing East (positive X direction)
    Wind wind(Vector2D(1.0f, 0.0f), 10.0f, true);
    environment->setWind(wind);
    
    // Ignite western cell
    grid->getCell(10, 10).ignite();
    
    fireModel->setBaseSpreadRate(0.4f);
    
    // Run simulation
    for (int i = 0; i < 20; ++i) {
        fireModel->update(*grid, *environment, 0.1f);
    }
    
    // Check spread pattern - should spread more to the east
    int eastBurning = 0;
    int westBurning = 0;
    
    for (size_t x = 11; x < 15; ++x) {
        if (grid->getCell(x, 10).isBurning() || 
            grid->getCell(x, 10).getState() == CellState::Burned) {
            eastBurning++;
        }
    }
    
    for (size_t x = 6; x < 10; ++x) {
        if (grid->getCell(x, 10).isBurning() || 
            grid->getCell(x, 10).getState() == CellState::Burned) {
            westBurning++;
        }
    }
    
    // Should spread more downwind (east) than upwind (west)
    EXPECT_GT(eastBurning, westBurning);
}

TEST_F(FireModelTest, DisabledWindHasNoEffect) {
    // Set strong wind but disabled
    Wind wind(Vector2D(1.0f, 0.0f), 20.0f, false);
    environment->setWind(wind);
    
    grid->getCell(10, 10).ignite();
    fireModel->setBaseSpreadRate(0.4f);
    
    // Run simulation
    for (int i = 0; i < 15; ++i) {
        fireModel->update(*grid, *environment, 0.1f);
    }
    
    // Count spread in all directions
    int north = grid->getCell(10, 9).isBurning() ? 1 : 0;
    int south = grid->getCell(10, 11).isBurning() ? 1 : 0;
    int east = grid->getCell(11, 10).isBurning() ? 1 : 0;
    int west = grid->getCell(9, 10).isBurning() ? 1 : 0;
    
    // With disabled wind, spread should be relatively uniform
    // At least 2 directions should have spread
    EXPECT_GE(north + south + east + west, 2);
}

// Weather Effect Tests
TEST_F(FireModelTest, HighHumidityReducesSpread) {
    Weather dryWeather;
    dryWeather.humidity = 0.1f;
    environment->setWeather(dryWeather);
    
    grid->getCell(5, 5).ignite();
    fireModel->setBaseSpreadRate(0.5f);
    
    for (int i = 0; i < 10; ++i) {
        fireModel->update(*grid, *environment, 0.1f);
    }
    
    size_t drySpread = grid->countIf([](const Cell& c) { return c.isBurning(); });
    
    // Reset and try with high humidity
    grid->reset();
    Weather humidWeather;
    humidWeather.humidity = 0.9f;
    environment->setWeather(humidWeather);
    
    grid->getCell(5, 5).ignite();
    
    for (int i = 0; i < 10; ++i) {
        fireModel->update(*grid, *environment, 0.1f);
    }
    
    size_t humidSpread = grid->countIf([](const Cell& c) { return c.isBurning(); });
    
    // Dry conditions should result in more spread
    EXPECT_GT(drySpread, humidSpread);
}

TEST_F(FireModelTest, RainReducesSpread) {
    Weather noRain;
    noRain.precipitation = 0.0f;
    environment->setWeather(noRain);
    
    grid->getCell(5, 5).ignite();
    fireModel->setBaseSpreadRate(0.5f);
    
    for (int i = 0; i < 10; ++i) {
        fireModel->update(*grid, *environment, 0.1f);
    }
    
    size_t drySpread = grid->countIf([](const Cell& c) { return c.isBurning(); });
    
    // Reset and try with rain
    grid->reset();
    Weather rainyWeather;
    rainyWeather.precipitation = 5.0f; // Heavy rain
    environment->setWeather(rainyWeather);
    
    grid->getCell(5, 5).ignite();
    
    for (int i = 0; i < 10; ++i) {
        fireModel->update(*grid, *environment, 0.1f);
    }
    
    size_t rainySpread = grid->countIf([](const Cell& c) { return c.isBurning(); });
    
    // Rain should significantly reduce spread
    EXPECT_GT(drySpread, rainySpread);
}

// Determinism Tests
TEST_F(FireModelTest, DeterministicWithSameSeed) {
    // First run
    SimpleFireModel model1(42);
    Grid grid1(15, 15);
    Environment env1;
    
    grid1.getCell(7, 7).ignite();
    model1.setBaseSpreadRate(0.4f);
    
    for (int i = 0; i < 10; ++i) {
        model1.update(grid1, env1, 0.1f);
    }
    
    size_t burning1 = grid1.countIf([](const Cell& c) { return c.isBurning(); });
    
    // Second run with same seed
    SimpleFireModel model2(42);
    Grid grid2(15, 15);
    Environment env2;
    
    grid2.getCell(7, 7).ignite();
    model2.setBaseSpreadRate(0.4f);
    
    for (int i = 0; i < 10; ++i) {
        model2.update(grid2, env2, 0.1f);
    }
    
    size_t burning2 = grid2.countIf([](const Cell& c) { return c.isBurning(); });
    
    // Same seed should produce identical results
    EXPECT_EQ(burning1, burning2);
}

// Performance Tests
TEST_F(FireModelTest, HandlesLargeGrid) {
    Grid largeGrid(100, 100);
    
    // Ignite multiple points
    largeGrid.getCell(25, 25).ignite();
    largeGrid.getCell(50, 50).ignite();
    largeGrid.getCell(75, 75).ignite();
    
    fireModel->setBaseSpreadRate(0.3f);
    
    // Should complete without hanging
    for (int i = 0; i < 20; ++i) {
        fireModel->update(largeGrid, *environment, 0.1f);
    }
    
    // Just verify it completed
    EXPECT_TRUE(true);
}

// Edge Cases
TEST_F(FireModelTest, ZeroDeltaTime) {
    grid->getCell(10, 10).ignite();
    
    fireModel->update(*grid, *environment, 0.0f);
    
    // Should not spread with zero delta time
    size_t burning = grid->countIf([](const Cell& c) { return c.isBurning(); });
    EXPECT_EQ(burning, 1); // Only the initial cell
}

TEST_F(FireModelTest, SmolderingCellsCanSpread) {
    // Manually set a cell to smoldering
    grid->getCell(10, 10).setState(CellState::Smoldering);
    grid->getCell(10, 10).setTemperature(350.0f);
    
    fireModel->setBaseSpreadRate(1.0f);
    
    // Update multiple times
    for (int i = 0; i < 20; ++i) {
        fireModel->update(*grid, *environment, 0.1f);
    }
    
    // Smoldering cells should be able to spread (though slower)
    size_t burning = grid->countIf([](const Cell& c) { return c.isBurning(); });
    EXPECT_GT(burning, 1);
}