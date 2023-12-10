#ifndef IC_EVENTS_H
#define IC_EVENTS_H

#include <algorithm>
#include <string>
#include <map>
#include <vector>
#include <iostream>


namespace ic {
    class Events {
        public:
            static Events& get() {
                static Events ins;
                return ins;
            }

            Events& on(const std::string &name, void (*callback)()) {
                std::vector<void (*)()> &listeners = this->events[name];

                if (std::find(listeners.begin(), listeners.end(), callback) != listeners.end()) {
                    std::cout << "Event " << name << " already has the same function!" << "\n";
                    return *this;
                }

                listeners.emplace_back(callback);

                return *this;
            }


            bool fire(const std::string &name) {
                std::vector<void (*)()> &listeners = this->events[name];

                if (listeners.size() == 0) return false;

                for (int i = 0; i < listeners.size(); i++) {
                    listeners[i]();
                }

                return true;
            }


        private:
            std::map<std::string, std::vector<void (*)()>> events;

        private:
            Events() {}
            ~Events() {}
        public:
            Events(Events const&) = delete;
            void operator = (Events const&) = delete;  
    };
}

#endif