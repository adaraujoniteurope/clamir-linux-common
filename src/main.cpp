#include <atomic>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <syslog.h>

#include <netinet/if_fddi.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>

#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <csignal>

#include <nit/embedded/clamir_service_dbus.hpp>

using namespace nit::embedded;
std::shared_ptr<clamir_service_dbus> server;

int main(int argc, char *argv[]) {

  static std::atomic_bool shutdown = false;

  std::signal(SIGINT, [](int) { shutdown.store(true); });
  std::signal(SIGTERM, [](int) { shutdown.store(true); });
  std::signal(SIGPIPE, SIG_IGN);

  server = clamir_service_dbus::get_instance();

  try {
    auto retval = server->initialize(argc, argv);
    if (retval < 0) {
      exit(retval);
    }

  } catch (std::exception &ex) {
    syslog(LOG_INFO, "%s", ex.what());
  }

  try {
    server->run(shutdown);
  } catch (std::exception &ex) {
    syslog(LOG_INFO, "%s", ex.what());
  }

  clamir_service_dbus applcation;

  return EXIT_SUCCESS;
}