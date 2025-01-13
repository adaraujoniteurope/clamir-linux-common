#ifndef NETWORKING_PROFINET_DEVICE_H_
#define NETWORKING_PROFINET_DEVICE_H_

#include <cstdint>
#include <functional>
#include <atomic>

class device
{
public:
  using handler_type = std::function<void()>;
  static handler_type create(const char* hwaddr, void* pdata, std::atomic_bool& shutdown);

private:
  device() = delete;
  device(device &) = delete;
  device(device &&) = delete;
};

#endif