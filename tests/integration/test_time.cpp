#include "cascade/core/time.hpp"
#include <gtest/gtest.h>
#include <thread>
#include <chrono>

using namespace cascade;

TEST(TimeTest, InitialFrameCountIsZero)
{
    Time time;
    EXPECT_EQ(time.getFrameCount(), 0);
}

TEST(TimeTest, InitialElapsedTimeIsZero)
{
    Time time;
    EXPECT_EQ(time.getElapsedTime(), 0.0f);
}

TEST(TimeTest, StartInitializesTime)
{
    Time time;
    time.start();

    EXPECT_EQ(time.getFrameCount(), 0);
    EXPECT_GE(time.getElapsedTime(), 0.0f);
}

TEST(TimeTest, UpdateIncrementsFrameCount)
{
    Time time;
    time.start();

    time.update();
    EXPECT_EQ(time.getFrameCount(), 1);

    time.update();
    EXPECT_EQ(time.getFrameCount(), 2);
}

TEST(TimeTest, UpdateReturnsPositiveDeltaTime)
{
    Time time;
    time.start();

    // Small sleep to ensure measurable time passes
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    float dt = time.update();
    EXPECT_GT(dt, 0.0f) << "Delta time should be positive";
}

TEST(TimeTest, ElapsedTimeIncreases)
{
    Time time;
    time.start();

    float initialElapsed = time.getElapsedTime();

    // Small sleep to ensure time passes
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    time.update();

    float newElapsed = time.getElapsedTime();
    EXPECT_GT(newElapsed, initialElapsed) << "Elapsed time should increase after update";
}

TEST(TimeTest, MultipleUpdates)
{
    Time time;
    time.start();

    for (int i = 0; i < 5; ++i)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        float dt = time.update();

        EXPECT_GT(dt, 0.0f) << "Delta time should be positive on iteration " << i;
        EXPECT_EQ(time.getFrameCount(), i + 1) << "Frame count should match iteration";
    }
}

TEST(TimeTest, DeltaTimeApproximatelyMatchesRealTime)
{
    Time time;
    time.start();

    // Sleep for approximately 50ms
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    float dt = time.update();

    // Delta time should be roughly 0.05 seconds (50ms)
    // Allow for some variance due to system scheduling
    EXPECT_NEAR(dt, 0.05f, 0.02f) << "Delta time should approximately match sleep duration";
}