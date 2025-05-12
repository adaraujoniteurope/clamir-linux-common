#ifndef _nit_embedded_core_signal_hpp_
#define _nit_embedded_core_signal_hpp_

#include <functional>
#include <mutex>
#include <list>
#include <memory>
#include <nit/embedded/core/thread_pool.hpp>

namespace nit::embedded::core {
template <typename sender_type, typename... args_types> class signal {
public:
  signal() = default;

  signal(signal &) = delete;

  using slot_type = std::function<void(sender_type, args_types...)>;

  const typename std::list<slot_type>::iterator connect(const slot_type &slot) {
    std::lock_guard<std::mutex> lock(_mutex);
    return sinks.insert(sinks.end(), std::move(slot));
  }

  void disconnect(const typename std::list<slot_type>::iterator &it) {
    std::lock_guard<std::mutex> lock(_mutex);
    sinks.erase(it);
  }

  void disconnect(const slot_type &slot) {
    std::lock_guard<std::mutex> lock(_mutex);
    sinks.remove(slot);
  }

  void disconnect_all() {
    std::lock_guard<std::mutex> lock(_mutex);
    sinks.clear();
  }

  void operator()(sender_type sender, args_types... args) {
    std::lock_guard<std::mutex> lock(_mutex);
    auto args_tuple =
        std::make_shared<std::tuple<args_types...>>(std::move(args)...);
    auto sender_copy = sender;

    for (const auto &slot : sinks) {
      _thread_pool.submit([slot, sender_copy, args_tuple]() {
        std::apply(
            [&](auto &&...unpacked_args) {
              slot(sender_copy, unpacked_args...);
            },
            *args_tuple);
      });
    }
  }

  signal &operator+=(const slot_type &slot) {
    connect(slot);
    return *this;
  }

protected:
  std::mutex _mutex;
  std::list<slot_type> sinks;
  thread_pool &_thread_pool = thread_pool::get_instance();
};
} // namespace nit::embedded::core

#endif