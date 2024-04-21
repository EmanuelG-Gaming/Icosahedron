#include <Icosahedron/util/Time.h>


void ic::Time::tick(uint32_t current) {
    ic::Time::globalTime += ic::Time::globalDelta;
    ic::Time::time += ic::Time::delta;
    
    ic::Time::globalDelta = (current - ic::Time::lastUpdate) / 1000.0f;
    ic::Time::delta = ic::Time::globalDelta * ic::Time::deltaMultiplier; 
    ic::Time::lastUpdate = current;
}