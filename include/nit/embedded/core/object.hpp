#ifndef _nit_embedded_core_object_hpp_
#define _nit_embedded_core_object_hpp_

#include <memory>
#include <mutex>
#include <memory>

namespace nit::embedded::core
{
    template<typename type>
class object : public std::enable_shared_from_this<type>
{
public:
    void lock() { _mutex.lock(); }
    void unlock() { _mutex.unlock(); }
    auto try_lock() { return _mutex.try_lock(); }
    std::mutex& mutex_get() { return _mutex; }
private:
    std::mutex _mutex;
};
}


#endif