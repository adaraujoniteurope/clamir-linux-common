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
            for (auto slot : container)
            {
                // auto _ = std::async(std::launch::async, slot, std::forward<args_types>(args)...);
                slot(std::forward<args_types>(args)...);
            }
        }

        const signal& operator+=(slot_type slot)
        {
            append_listener(slot);
            return *this;
        }

        private:

        std::list<slot_type> container;
    };
}

#endif