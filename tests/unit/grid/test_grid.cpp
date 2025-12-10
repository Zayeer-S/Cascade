#include "cascade/grid/grid.hpp"
#include <gtest/gtest.h>

using namespace cascade;

class GridTest : public ::testing::Test {
protected:
    void SetUp() override {
        grid = new Grid(10, 10);
    }

    void TearDown() override {
        delete grid;
    }

    Grid* grid;
};

TEST_F(GridTest, Construction) {
    EXPECT_EQ(grid->getWidth(), 10);
    EXPECT_EQ(grid->getHeight(), 10);
    EXPECT_EQ(grid->getTotalCells(), 100);
}

TEST_F(GridTest, ConstructionInvalidDimensions) {
    EXPECT_THROW(Grid(0, 10), std::invalid_argument);
    EXPECT_THROW(Grid(10, 0), std::invalid_argument);
}

TEST_F(GridTest, GetCell) {
    Cell& cell = grid->getCell(5, 5);
    EXPECT_EQ(cell.getX(), 5);
    EXPECT_EQ(cell.getY(), 5);
}

TEST_F(GridTest, GetCellOutOfBounds) {
    EXPECT_THROW(grid->getCell(10, 5), std::out_of_range);
    EXPECT_THROW(grid->getCell(5, 10), std::out_of_range);
}

TEST_F(GridTest, IsValid) {
    EXPECT_TRUE(grid->isValid(0, 0));
    EXPECT_TRUE(grid->isValid(9, 9));
    EXPECT_FALSE(grid->isValid(10, 5));
    EXPECT_FALSE(grid->isValid(5, 10));
}

TEST_F(GridTest, GetNeighbours4Center) {
    auto neighbors = grid->getNeighbours4(5, 5);
    EXPECT_EQ(neighbors.size(), 4);
}

TEST_F(GridTest, GetNeighbours4Corner) {
    auto neighbors = grid->getNeighbours4(0, 0);
    EXPECT_EQ(neighbors.size(), 2); 
}

TEST_F(GridTest, GetNeighbours4Edge) {
    auto neighbors = grid->getNeighbours4(5, 0);
    EXPECT_EQ(neighbors.size(), 3);   
}

TEST_F(GridTest, GetNeighbours8Center) {
    auto neighbors = grid->getNeighbours8(5, 5);
    EXPECT_EQ(neighbors.size(), 8);
}

TEST_F(GridTest, GetNeighbours8Corner) {
    auto neighbors = grid->getNeighbours8(0, 0);
    EXPECT_EQ(neighbors.size(), 3);
}

TEST_F(GridTest, GetCellsInRadius) {
    auto cells = grid->getCellsInRadius(5, 5, 2.0f);
    EXPECT_GT(cells.size(), 0);
    EXPECT_LE(cells.size(), 25);
}

TEST_F(GridTest, GetCellsInRadiusZero) {
    auto cells = grid->getCellsInRadius(5, 5, 0.0f);
    EXPECT_EQ(cells.size(), 1);
}

TEST_F(GridTest, ForEach) {
    int count = 0;
    grid->forEach([&count](Cell& cell) {
        (void)cell;
        count++;
    });
    EXPECT_EQ(count, 100);
}

TEST_F(GridTest, ForEachConst) {
    const Grid* constGrid = grid;
    int count = 0;
    constGrid->forEach([&count](const Cell& cell) {
        (void)cell;
        count++;
    });
    EXPECT_EQ(count, 100);
}

TEST_F(GridTest, ForEachIf) {
    grid->getCell(0, 0).ignite();
    grid->getCell(5, 5).ignite();
    grid->getCell(9, 9).ignite();
    
    int burningCount = 0;
    grid->forEachIf(
        [](const Cell& c) { return c.isBurning(); },
        [&burningCount](Cell& c) { 
            (void)c;
            burningCount++; 
        }
    );
    
    EXPECT_EQ(burningCount, 3);
}

TEST_F(GridTest, CountIf) {
    grid->getCell(2, 2).ignite();
    grid->getCell(7, 7).ignite();
    
    size_t count = grid->countIf([](const Cell& c) {
        return c.isBurning();
    });
    
    EXPECT_EQ(count, 2);
}

TEST_F(GridTest, GetCellsByState) {
    grid->getCell(1, 1).setState(CellState::Burning);
    grid->getCell(3, 3).setState(CellState::Burning);
    grid->getCell(5, 5).setState(CellState::Wet);
    
    auto burning = grid->getCellsByState(CellState::Burning);
    EXPECT_EQ(burning.size(), 2);
    
    auto wet = grid->getCellsByState(CellState::Wet);
    EXPECT_EQ(wet.size(), 1);
    
    auto unburned = grid->getCellsByState(CellState::Unburned);
    EXPECT_EQ(unburned.size(), 97);
}

TEST_F(GridTest, Reset) {
    grid->getCell(0, 0).ignite();
    grid->getCell(5, 5).ignite();
    grid->getCell(9, 9).setState(CellState::Wet);
    
    grid->reset();
    
    size_t unburned = grid->countIf([](const Cell& c) {
        return c.getState() == CellState::Unburned;
    });
    EXPECT_EQ(unburned, 100);
}

TEST_F(GridTest, FireSpreadSimulation) {
    grid->getCell(5, 5).ignite();
    
    grid->forEach([](Cell& c) { c.update(0.1f); });
    
    EXPECT_TRUE(grid->getCell(5, 5).isBurning());
}

TEST_F(GridTest, LargeGrid) {
    Grid large(100, 100);
    EXPECT_EQ(large.getTotalCells(), 10000);
    
    EXPECT_NO_THROW(large.getCell(0, 0));
    EXPECT_NO_THROW(large.getCell(99, 99));
}

TEST_F(GridTest, SmallGrid) {
    Grid small(3, 3);
    EXPECT_EQ(small.getTotalCells(), 9);
    
    auto neighbors = small.getNeighbours4(1, 1);
    EXPECT_EQ(neighbors.size(), 4);
}