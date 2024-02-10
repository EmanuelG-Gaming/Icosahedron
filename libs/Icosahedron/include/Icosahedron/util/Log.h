#ifndef IC_LOG_H
#define IC_LOG_H

#include <string>
#include <iostream>


namespace ic {
    enum class LogLevels {
        NONE,
        INFO,
        DEBUG,
        WARN,
        ERROR,
    };

    class Log {
        public:
            static Log& get() {
                static Log ins;
                return ins;
            }

            void log(const LogLevels &level, const std::string &message) {
                if (message.empty()) return;
                
                std::cout << message;

                content.append(message);
            }




            void info(const std::string &message) {
                this->log(ic::LogLevels::INFO, message);
            }

            void debug(const std::string &message) {
                this->log(ic::LogLevels::DEBUG, message);
            }

            void warn(const std::string &message) {
                this->log(ic::LogLevels::WARN, message);
            }

            void error(const std::string &message) {
                this->log(ic::LogLevels::ERROR, message);
            }

        private:
            std::string content;

        private:
            Log() {}
            ~Log() {}

        public:
            Log(Log const&) = delete;
            void operator = (Log const&) = delete;

    };
};

extern void IC_LOG(const ic::LogLevels &level, const std::string &message) {
    ic::Log::get().log(level, message);
}

extern void IC_INFO(const std::string &message) {
    ic::Log::get().info(message);
}

extern void IC_DEBUG(const std::string &message) {
    ic::Log::get().debug(message);
}

extern void IC_WARN(const std::string &message) {
    ic::Log::get().warn(message);
}

extern void IC_ERROR(const std::string &message) {
    ic::Log::get().error(message);
}


#endif