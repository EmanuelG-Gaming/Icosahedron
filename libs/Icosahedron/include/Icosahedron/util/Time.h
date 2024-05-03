#ifndef IC_TIME_H
#define IC_TIME_H

#include <cstdint>
#include <functional>
#include <thread>
#include <chrono>
#include <iostream>


namespace ic {
    struct TimedThread {
        void operator()(float timeSeconds, float snapshot, const std::function<void()> &cons);
    };

    namespace Time {
        /** @brief The time elapsed since startup in seconds. */
        extern float globalTime;

        /** @brief The time elapsed since startup in seconds. 
         *  This can be affected by the delta multiplier.
        */
        extern float time;

        extern float globalDelta, delta;

        extern float deltaMultiplier;

        extern uint32_t lastUpdate;

        void tick(uint32_t current);

        /** @brief Opens a new thread that waits for an instruction for a certain amount of time. */
        void wait(float seconds, const std::function<void()> &cons);
    };
}

#endif