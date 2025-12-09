#include "cascade/grid/Cell.hpp"
#include <gtest/gtest.h>

using namespace cascade;

class CellTest : public ::testing::Test {
protected:
    void SetUp() override {
        cell = new Cell(5, 10);
    }

    void TearDown() override {
        delete cell;
    }

    Cell* cell;
};

TEST_F(CellTest, Construction) {
    EXPECT_EQ(cell->getX(), 5);
    EXPECT_EQ(cell->getY(), 10);
    EXPECT_EQ(cell->getState(), CellState::Unburned);
    EXPECT_GT(cell->getFuelAmount(), 0.0f);
}

TEST_F(CellTest, InitialState) {
    EXPECT_FALSE(cell->isBurning());
    EXPECT_TRUE(cell->canIgnite());
}

TEST_F(CellTest, Ignition) {
    EXPECT_TRUE(cell->ignite());
    EXPECT_TRUE(cell->isBurning());
    EXPECT_FALSE(cell->canIgnite());
}

TEST_F(CellTest, CannotIgniteTwice) {
    cell->ignite();
    EXPECT_FALSE(cell->ignite());
}

TEST_F(CellTest, BurningConsumesFuel) {
    cell->ignite();
    float initial = cell->getFuelAmount();
    cell->update(1.0f);
    EXPECT_LT(cell->getFuelAmount(), initial);
}

TEST_F(CellTest, BurnoutTransitionsToBurned) {
    cell->ignite();
    for (int i = 0; i < 200; ++i) {
        cell->update(0.1f);
        if (cell->getState() == CellState::Burned) break;
    }
    EXPECT_EQ(cell->getState(), CellState::Burned);
    EXPECT_FALSE(cell->isBurning());
}

TEST_F(CellTest, ExtinguishBurningCell) {
    cell->ignite();
    EXPECT_TRUE(cell->extinguish(10.0f));
    EXPECT_FALSE(cell->isBurning());
}

TEST_F(CellTest, PreventiveWetting) {
    EXPECT_TRUE(cell->extinguish(5.0f));
    EXPECT_EQ(cell->getState(), CellState::Wet);
}

TEST_F(CellTest, Reset) {
    cell->ignite();
    cell->update(5.0f);
    cell->reset();
    EXPECT_EQ(cell->getState(), CellState::Unburned);
    EXPECT_GT(cell->getFuelAmount(), 0.9f);
}