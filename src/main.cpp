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

#include "application.hpp"

std::shared_ptr<application> app;

int main(int argc, char *argv[])
{

	app = application::get_instance();

	try
	{
		app->initialize(argc, argv);
	}
	catch (std::exception &ex)
	{
		std::cout << ex.what() << std::endl;
	}

	try
	{
		app->run();
	}
	catch (std::exception &ex)
	{
		std::cout << ex.what() << std::endl;
	}

	return EXIT_SUCCESS;
}