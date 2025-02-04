#ifndef _COMPONENTS_TIMER_HPP_
#define _COMPONENTS_TIMER_HPP_

#include <thread>
#include <condition_variable>
#include <functional>
#include <atomic>
#include <iostream>

#include <utils/waitable.hpp>
#include <utils/runnable_worker.hpp>

#include <utils/signal.hpp>
#include <fcntl.h>

class abstract_timer : public utils::waitable, public utils::runnable_worker
{
public:

    utils::signal<abstract_timer*, long> elapsed;

    abstract_timer(long ns, std::atomic_bool &shutdown) : m_interval(ns), m_shutdown(shutdown) {}
    virtual ~abstract_timer() { std::cout << __func__ << std::endl; }

    virtual long now() = 0;

    void wait() override
    {
        std::unique_lock<std::mutex> lk(m_timer_mutex);
        m_timer_cv.wait(lk);
    }

    void shutdown()
    {
        m_shutdown.store(true);
    }

protected:

    long m_interval;
    std::atomic_bool& m_shutdown;

    void callback(std::atomic_bool &shutdown)
    {
        std::unique_lock<std::mutex> lk(m_timer_mutex);
        m_timer_cv.notify_all();
        elapsed.emit(this, now());
    }

private:
    std::condition_variable m_timer_cv;
    std::mutex m_timer_mutex;
};


class uio_timer : public abstract_timer
{
public:

    uio_timer(std::atomic_bool &shutdown) : abstract_timer(std::numeric_limits<long>::min(), shutdown) {}
    ~uio_timer() { std::cout << __func__ << std::endl; }
    
    long now() override
    {
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        return (long)ts.tv_sec * 1000000000LL + ts.tv_nsec;
    }

    void run() override
    {
        fd = open("/dev/uio0", O_RDWR);

        if (fd < 0) {
            throw std::runtime_error("Couldn't open /dev/ui0");
        }

        int enable = 1;
        int pending = 0;
        
        while (!m_shutdown.load())
        {
            write(fd, (void *)&enable, sizeof(int));
            read(fd, (int *)&pending, sizeof(int));
            callback(m_shutdown);
        }
    }

    private:
    int fd = -1;
    unsigned long start;
};

class linux_rtc_timer : public abstract_timer
{
public:

    linux_rtc_timer(long ns, std::atomic_bool &shutdown) : abstract_timer(ns, shutdown) {}
    ~linux_rtc_timer() { std::cout << __func__ << std::endl; }
    
    long now() override
    {
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        return (long)ts.tv_sec * 1000000000LL + ts.tv_nsec;
    }

    void run() override
    {
        callback(m_shutdown);

        long nanoseconds = m_interval * 1000UL;

        while (!m_shutdown.load())
        {

            long start = now();
            callback(m_shutdown);
            long elapsed = now() - start;

            if (elapsed < nanoseconds)
            {
                long sleep_ns = nanoseconds - elapsed;

                struct timespec sleep_time = {
                    .tv_sec = sleep_ns / 1000000000L,
                    .tv_nsec = sleep_ns % 1000000000L};

                nanosleep(&sleep_time, NULL);
            }
        }
    }
};

#endif