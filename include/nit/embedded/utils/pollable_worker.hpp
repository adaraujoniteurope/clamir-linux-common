#ifndef _UTILS_POLLABLE_WORKER_HPP_
#define _UTILS_POLLABLE_WORKER_HPP_

#include <nit/embedded/utils/waitable.hpp>
#include <nit/embedded/utils/pollable.hpp>

#include <functional>
#include <atomic>

namespace utils
{
    class pollable_worker : public pollable
    {
    public:
        virtual ~pollable_worker() {
            syslog(LOG_INFO, __func__);
        }
        template <class... types>
        std::function<void()> get_worker(std::atomic<bool> &shutdown, waitable& timer, types... args)
        {
            return [this, &shutdown = shutdown, &timer = timer, args...]() -> void
            {
                while (!shutdown.load())
                {
                    timer.wait();
                    poll(std::forward<decltype(args)>(args)...);
                }
            };
        }
    };
}

#endif