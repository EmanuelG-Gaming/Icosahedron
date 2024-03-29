#ifndef IC_TIME_H
#define IC_TIME_H

namespace ic {
    namespace Time {
        /** @brief The time elapsed since startup in seconds. */
        extern float globalTime;

        /** @brief The time elapsed since startup in seconds. 
         *  This can be affected by the delta multiplier.
        */
        extern float time;

        extern float globalDelta, delta;

        extern float deltaMultiplier;
    };
}

#endif