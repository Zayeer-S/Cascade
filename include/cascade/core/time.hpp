#ifndef CASCADE_CORE_TIME_HPP
#define CASCADE_CORE_TIME_HPP

#include <chrono>

namespace cascade
{

    /**
     * @brief Time management class for simulation timing
     *
     * Handles data time calculations, frame counting, and time stepping for the simulation engine
     */
    class Time
    {
    public:
        using Clock = std::chrono::steady_clock;
        using TimePoint = std::chrono::time_point<Clock>;
        using Duration = std::chrono::duration<float>;

        /**
         * @brief Constructor with optional fixed time step
         * @param fixedTimeStep if > 0, use fixed time step; otherwise variable
         */
        explicit Time(float fixedTimeStep = 0.0f);

        /**
         * @brief Start the timer
         */
        void start();

        /**
         * @brief Updated the timer once per frame
         * @return Delta time in seconds
         */
        float update();

        /**
         * @brief Reset the timer
         */
        void reset();

        /**
         * @brief Get the delta time from the last update
         * @return Delta time in seconds
         */
        float getDeltaTime() const { return deltaTime_; }

        /**
         * @brief Get total elapsed time since start
         * @return Total time in seconds
         */
        float getElapsedTime() const { return elapsedTime_; }

        /**
         * @brief Get the frame count
         * @return Number of frames since start
         */
        unsigned int getFrameCount() const { return frameCount_;}

        /**
         * @brief Get current frames per second
         * @return FPS (averaged over last second)
         */
        float getFPS() const { return fps_; }

        /**
         * @brief CHeck if using fixed time step
         * @return true if fixed time step is enabled
         */
        bool isFixedTimeStep() const { return fixedTimeStep_; }

        /**
         * @brief Set fixed time step
         * @param timeStep Fixed time step in seconds (0 to disable)
         */
        void setFixedTimeStep(float timeStep);

    private:
        void updateFPS();

        TimePoint startTime_;
        TimePoint lastFrameTime_;

        float deltaTime_;
        float elapsedTime_;
        float fixedTimeStep_;
        bool useFixedTimeStep_;
        
        unsigned int frameCount_;

        float fps_;
        unsigned int framesSinceLastFPSUpdate_;
        float timeSinceLastUpdate_;
    };
}

#endif