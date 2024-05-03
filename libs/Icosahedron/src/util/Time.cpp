#include <Icosahedron/util/Time.h>


void ic::TimedThread::operator()(float timeSeconds, float snapshot, const std::function<void()> &cons) {
    //bool enabled = true;

    //while (enabled) {
    //    float delta = ic::Time::time - snapshot;
//
    //    if (delta >= timeSeconds) {
    //        enabled = false;
            cons();
            std::cout << "the" << "\n";


    //       break;
    //    }
    //}
}

void ic::Time::tick(uint32_t current) {
    ic::Time::globalTime += ic::Time::globalDelta;
    ic::Time::time += ic::Time::delta;
    
    ic::Time::globalDelta = (current - ic::Time::lastUpdate) / 1000.0f;
    ic::Time::delta = ic::Time::globalDelta * ic::Time::deltaMultiplier; 
    ic::Time::lastUpdate = current;
}

void ic::Time::wait(float seconds, const std::function<void()> &cons) {
    std::thread threadObject(ic::TimedThread(), seconds, ic::Time::time, cons);
    threadObject.join();
}