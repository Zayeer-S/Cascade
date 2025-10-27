#include "cascade/core/time.hpp"
#include <algorithm>

namespace cascade
{
    Time::Time(float fixedTimeStep)
        : deltaTime_(0.0f), elapsedTime_(0.0f), fixedTimeStep_(fixedTimeStep), useFixedTimeStep_(fixedTimeStep > 0.0f), frameCount_(0), fps_(0.0f), framesSinceLastFPSUpdate_(0), timeSinceLastUpdate_(0.0f)
    {
    }

    void Time::start()
    {
        startTime_ = Clock::now();
        lastFrameTime_ = startTime_;
        deltaTime_ = 0.0f;
        elapsedTime_ = 0.0f;
        frameCount_ = 0;
        fps_ = 0.0f;
        framesSinceLastFPSUpdate_ = 0;
        timeSinceLastUpdate_ = 0.0f;
    }

    float Time::update()
    {
        TimePoint currentTime = Clock::now();

        if (useFixedTimeStep_)
        {
            deltaTime_ = fixedTimeStep_;
        }
        else
        {
            Duration frameDuration = currentTime - lastFrameTime_;
            deltaTime_ = frameDuration.count();

            deltaTime_ = std::min(deltaTime_, 0.25f);
        }

        lastFrameTime_ = currentTime;
        elapsedTime_ += deltaTime_;
        frameCount_++;

        updateFPS();

        return deltaTime_;
    }

    void Time::reset()
    {
        start();
    }

    void Time::setFixedTimeStep(float timeStep)
    {
        fixedTimeStep_ = timeStep;
        useFixedTimeStep_ = (timeStep > 0.0f);
    }

    void Time::updateFPS()
    {
        framesSinceLastFPSUpdate_++;
        timeSinceLastUpdate_ += deltaTime_;

        if (timeSinceLastUpdate_ >= 1.0f)
        {
            fps_ = static_cast<float>(framesSinceLastFPSUpdate_) / timeSinceLastUpdate_;
            framesSinceLastFPSUpdate_ = 0;
            timeSinceLastUpdate_ = 0.0f;
        }
    }

} // namespace cascade
