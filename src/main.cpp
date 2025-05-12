#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include <netinet/if_fddi.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>

#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <iostream>
#include <thread>
#include <chrono>

#include <nit/embedded/server_ctrl_dbus.hpp>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

std::shared_ptr<server_ctrl_dbus> server;

int main(int argc, char *argv[])
{

	server = server_ctrl_dbus::get_instance();

	try
	{
		auto retval = server->initialize(argc, argv);
		if (retval < 0) {
			exit(retval);
		}

	}
	catch (std::exception &ex)
	{
		syslog(LOG_INFO, "%s", ex.what());
	}

	try
	{
		server->run();
	}
	catch (std::exception &ex)
	{
		syslog(LOG_INFO, "%s", ex.what());
	}

	return EXIT_SUCCESS;
}