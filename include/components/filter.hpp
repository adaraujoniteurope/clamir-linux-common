#ifndef _COMPONENTS_FILTER_HPP_
#define _COMPONENTS_FILTER_HPP_

#include <thread>
#include <condition_variable>
#include <functional>
#include <atomic>

class filter_base
{

public:
    filter_base(long interval, std::atomic_bool &shutdown) : m_interval(interval), m_shutdown(shutdown) {}

    void run()
    {
        long last = get_current_time_ns();
        callback(m_shutdown);
        long current = get_current_time_ns();

        long nanoseconds = m_interval * 1000UL;

        while (!m_shutdown.load())
        {

            long start = get_current_time_ns();
            callback(m_shutdown);
            long elapsed = get_current_time_ns() - start;

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

    void shutdown()
    {
        m_shutdown.store(true);
    }

    long get_current_time_ns()
    {
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        return (long)ts.tv_sec * 1000000000LL + ts.tv_nsec;
    }
protected:

    virtual void callback(std::atomic_bool& shutdown) = 0;

private:
    long m_interval;
    std::atomic_bool &m_shutdown;
};

class filter : public filter_base
{
public:
    filter(long interval, std::atomic_bool &shutdown) : filter_base(interval, shutdown)
    {
    }

    void wait()
    {
        std::unique_lock<std::mutex> lk(m_filter_mutex);
        m_filter_cv.wait(lk);
    }

    void callback(std::atomic_bool &shutdown) override
    {
        std::unique_lock<std::mutex> lk(m_filter_mutex);
        m_filter_cv.notify_all();
    }

    std::condition_variable m_filter_cv;
    std::mutex m_filter_mutex;
};

#endif