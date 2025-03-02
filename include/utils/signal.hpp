#ifndef _utils_signal_hpp_
#define _utils_signal_hpp_

#include <list>
#include <future>

namespace utils
{

    template<typename... args_types>
    class signal
    {
        public:

        using slot_type = std::function<void(args_types...)>;

        void append_listener(slot_type slot)
        {
            container.push_back(slot);
        }

        void remove_listener(slot_type& slot)
        {
            // not implemented
        }
        
        void emit(args_types...args) {

            std::mutex slot_mutex;
            std::unique_lock<std::mutex> lk(container_mutex);

            for (auto& slot : container)
            {
                std::unique_lock<std::mutex> slot_lock(slot_mutex);
                slot(std::forward<args_types>(args)...);
            }
        }

        const signal& operator+=(slot_type slot)
        {
            append_listener(slot);
            return *this;
        }

        private:

        std::mutex container_mutex;
        std::list<slot_type> container;
    };

    template<typename... args_types>
    class async_signal
    {
        public:

        using slot_type = std::function<void(args_types...)>;

        void append_listener(slot_type slot)
        {
            container.push_back(slot);
        }

        void remove_listener(slot_type& slot)
        {
            // not implemented
        }
        
        void emit(args_types...args) {

            std::mutex slot_mutex;
            std::unique_lock<std::mutex> lk(container_mutex);

            for (auto& slot : container)
            {
                std::unique_lock<std::mutex> slot_lock(slot_mutex);
                auto _ = std::async(std::launch::async, slot, std::forward<args_types>(args)...);
            }
        }

        const async_signal& operator+=(slot_type slot)
        {
            append_listener(slot);
            return *this;
        }

        private:

        std::mutex container_mutex;
        std::list<slot_type> container;
    };
}

#endif