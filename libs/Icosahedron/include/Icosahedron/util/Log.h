#ifndef IC_LOG_H
#define IC_LOG_H

#include <string>
#include <iostream>


namespace ic {
    enum class LogLevels : std::size_t {
        NONE = 0,
        INFO,
        DEBUG,
        WARN,
        ERROR,

        SIZE
    };

    namespace Log {
        void log(const LogLevels &level, const std::string &message) {
            if (message.empty()) return;
            
            std::cout << message;

            content.append(message);
        }




        void info(const std::string &message) {
            log(ic::LogLevels::INFO, message);
        }

        void debug(const std::string &message) {
            log(ic::LogLevels::DEBUG, message);
        }

        void warn(const std::string &message) {
            log(ic::LogLevels::WARN, message);
        }

        void error(const std::string &message) {
            log(ic::LogLevels::ERROR, message);
        }

        namespace {
            std::string content;
        };
    };
};

extern void IC_LOG(const ic::LogLevels &level, const std::string &message) {
    ic::Log::log(level, message);
}

extern void IC_INFO(const std::string &message) {
    ic::Log::info(message);
}

extern void IC_DEBUG(const std::string &message) {
    ic::Log::debug(message);
}

extern void IC_WARN(const std::string &message) {
    ic::Log::warn(message);
}

extern void IC_ERROR(const std::string &message) {
    ic::Log::error(message);
}


#endif