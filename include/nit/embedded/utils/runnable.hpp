#ifndef UTILS_RUNNABLE_HPP_
#define UTILS_RUNNABLE_HPP_

#include <syslog.h>

namespace nit::embedded::utils
{
    class runnable
    {
    public:
        virtual ~runnable() { syslog(LOG_INFO, __func__); }
        virtual void run() = 0;
    };
};

#endif