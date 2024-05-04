#include <Icosahedron/util/Time.h>

void ic::Time::tick(uint32_t current) {
    ic::Time::globalTime += ic::Time::globalDelta;
    ic::Time::time += ic::Time::delta;
    
    ic::Time::globalDelta = (current - ic::Time::lastUpdate) / 1000.0f;
    ic::Time::delta = ic::Time::globalDelta * ic::Time::deltaMultiplier; 
    ic::Time::lastUpdate = current;

    for (int i = 0; i < running.size(); i++) {
        ic::DelayedRun *run = running[i];
        float delta = ic::Time::time - run->snapshot;

        if (delta >= run->delay) {
            run->runAtEnd();

            running.erase(running.begin() + i);
            delete run;
        }
    }
}

void ic::Time::run(float seconds, const std::function<void()> &cons) {
    ic::DelayedRun *delayed = new ic::DelayedRun();
    delayed->delay = seconds;
    delayed->snapshot = ic::Time::time;
    delayed->runAtEnd = cons;

    running.emplace_back(delayed);
}