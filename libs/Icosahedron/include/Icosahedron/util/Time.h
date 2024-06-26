#ifndef IC_TIME_H
#define IC_TIME_H

#include <cstdint>
#include <functional>


namespace ic {
    struct DelayedRun {
        float delay;
        float snapshot;
        std::function<void()> runAtEnd;
    };

    namespace Time {
        namespace {
            std::vector<DelayedRun*> running;
        };


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

        void run(float seconds, const std::function<void()> &cons);
    };
}

#endif