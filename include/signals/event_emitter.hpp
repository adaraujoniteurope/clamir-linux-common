#ifndef SIGNALS_SIGNAL_H_
#define SIGNALS_SIGNAL_H_

#include <vector>
#include <unordered_map>
#include <functional>
#include <thread>

/**
 * this is a brutality
 */
template <typename... Args>
class event_emitter
{
public:

    using slot_type = std::function<void(Args...)>;

    void connect(const std::function<void(Args...)> &callback)
    {
        callbacks.push_back(callback);
    }

    void emit(Args... args)
    {
        for (const auto &callback : callbacks)
        {

            auto runner = std::thread([&](){
                callback(args...);
                threads.erase(std::this_thread::get_id());
            });

            threads[runner.get_id()] = std::move(runner);
        }
    }

private:
    std::vector<std::function<void(Args...)>> callbacks;
    std::unordered_map<std::thread::id, std::thread> threads;
};

#endif