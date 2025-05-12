#ifndef _UTILS_RUNNABLE_WORKER_HPP_
#define _UTILS_RUNNABLE_WORKER_HPP_

#include <nit/embedded/utils/waitable.hpp>
#include <nit/embedded/utils/runnable.hpp>

#include <functional>
#include <atomic>
#include <syslog.h>

namespace utils
{
    class runnable_worker : public runnable
    {
    public:
        virtual ~runnable_worker() { syslog(LOG_INFO, __func__); }
        template <class... types>
        std::function<void()> get_worker(types... args)
        {
            return [this, args...]() -> void
            {
                run(std::forward<decltype(args)>(args)...);
            };
        }
    };
}

#endif