#ifndef IC_EVENTS_H
#define IC_EVENTS_H

#include <algorithm>
#include <string>
#include <map>
#include <vector>
#include <iostream>


namespace ic {
    namespace Events {
        namespace {
            std::map<std::string, std::vector<void (*)()>> events;
        };
        
        void on(const std::string &name, void (*callback)()) {
            std::vector<void (*)()> &listeners = events[name];

            if (std::find(listeners.begin(), listeners.end(), callback) != listeners.end()) {
                std::cout << "Event " << name << " already has the same function!" << "\n";
                return;
            }

            listeners.emplace_back(callback);
        }


        bool fire(const std::string &name) {
            std::vector<void (*)()> &listeners = events[name];

            if (listeners.size() == 0) return false;

            for (int i = 0; i < listeners.size(); i++) {
                listeners[i]();
            }

            return true;
        }
    };
}

#endif