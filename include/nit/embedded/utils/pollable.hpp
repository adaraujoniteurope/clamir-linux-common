#ifndef UTILS_POLLABLE_HPP_
#define UTILS_POLLABLE_HPP_

namespace nit::embedded::utils
{
    class pollable
    {
    public:
        virtual ~pollable() {
            syslog(LOG_INFO, __func__);
        }
        virtual void poll() = 0;
    };
};

#endif