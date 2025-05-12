#ifndef _nit_embedded_core_thread_pool_hpp_
#define _nit_embedded_core_thread_pool_hpp_

#include <atomic>
#include <condition_variable>
#include <exception>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

namespace nit::embedded::core {

class thread_pool {
public:
  using task_type = std::function<void()>;

  void submit(task_type t) {
    std::unique_lock<std::mutex> lk{_mutex};
    _q.push(t);
    _q_cond.notify_one();
  }

  static thread_pool &get_instance() {
    static thread_pool __thread_pool;
    return __thread_pool;
  }

  thread_pool(int hardware_concurrency = std::thread::hardware_concurrency()) {
    /** push task evaluator (task_launcher in the thread pool) */
    for (int i = 0; i < hardware_concurrency; i++) {
      _pool.emplace_back(std::bind(&thread_pool::task_launcher, this));
    }
  }

  void shutdown() {
    _stop = true;
    _q_cond.notify_all();
    for (auto &t : _pool)
      t.join();
  }

  virtual ~thread_pool() { shutdown(); }

protected:
  void task_launcher() {
    while (!_stop) {

      std::function<void()> task;

      {
        auto lock = std::unique_lock<std::mutex>{_mutex};
        _q_cond.wait(lock, [this] { return !_q.empty() || _stop; });

        if (_stop && _q.empty())
          break;

        task = std::move(_q.front());
        _q.pop();
      }

      if (task) {
        try {
          task();
        } catch (std::exception &ex) {
          std::cerr << ex.what() << std::endl;
        } catch (...) {
          // Catch-all for safety
        }
      }
    }
  }

private:
  static thread_pool __thread_pool;
  std::queue<std::function<void()>> _q;
  std::condition_variable _q_cond;
  std::vector<std::thread> _pool;
  std::mutex _mutex;
  std::atomic_bool _stop = false;
};
} // namespace nit::embedded::core

#endif