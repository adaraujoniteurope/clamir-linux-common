#ifndef _NIT_EMBEDDE_UTILS_LOGGER_HPP_
#define _NIT_EMBEDDE_UTILS_LOGGER_HPP_

#include <mutex>
#include <queue>
#include <thread>

namespace nit::embedded::utils
{
    enum class logger_level
    {
        DEBUG,
        WARN,
        INFO,
    };

    class logger
    {
        private:
            logger() = delete;
            logger(logger&) = delete;
            logger(logger&&) = delete;
        public:

            template<typename ...Args>
            static void log(logger_level& level, const char * fmt, Args...args) {
                std::unique_lock<std::mutex> lk(mutex);
                lk.lock();
                // check forward syntax later...
                // std::bind(nullptr, printf, std::forward<Args...>(args));
            }
            static void debug() {}
            static void warn() {}
            static void info() {}

        private:
            static std::mutex mutex;
    };
}

#endif