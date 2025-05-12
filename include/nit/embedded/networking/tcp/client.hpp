#ifndef NETWORKING_TCP_SERVER_H_
#define NETWORKING_TCP_SERVER_H_

#include <string>
#include <cstdint>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <time.h>
#include <assert.h>

#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <pthread.h>
#include <sched.h>

#include <memory>
#include <future>
#include <atomic>

#include <ctime>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <format>
#include <future>
#include <map>

class server
{
public:
  using handler_type = std::function<void()>;
private:
  server() = delete;
  server(server &) = delete;
  server(server &&) = delete;
};

#endif