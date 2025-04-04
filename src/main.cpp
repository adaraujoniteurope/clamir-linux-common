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

#include <nit/embedded/application.hpp>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

std::shared_ptr<application> app;

// DRIVER_DECLARE_CONFIG_BEGIN(test_config)
// 	std::string name;
// 	short age;
// 	DRIVER_DECLARE_CONFIG_SERIALIZER_BEGIN(test_config)
// 		ar & boost::make_nvp("name", name);
// 		ar & boost::make_nvp("age", age);
// 	DRIVER_DECLARE_CONFIG_SERIALIZER_END(test_config)
// DRIVER_DECLARE_CONFIG_END(test_config)

// struct simple_object
// {
// 	std::string name;
// 	short age;

// 	template<typename archiver>
// 	void serialize(archiver& ar, unsigned)
// 	{
// 		ar & boost::make_nvp("name", name);
// 		ar & boost::make_nvp("age", age);
// 	}
// };

int main(int argc, char *argv[])
{
	// nit_test_config_config_t object { .name = "Example Object" };

	// {
	// 	std::ofstream ofs("nit_test_config_config_t.dat");
	// 	boost::archive::text_oarchive toa(ofs);
	// 	toa & object;
	// }

	// {
	// 	std::ifstream ifs("nit_test_config_config_t.dat");
	// 	boost::archive::text_iarchive tia(ifs);
	// 	tia & object;
	// }

	app = application::get_instance();

	try
	{
		auto retval = app->initialize(argc, argv);
		if (retval < 0) {
			exit(retval);
		}

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