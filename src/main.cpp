#include <nit/embedded/core/application.hpp>

int main(int argc, char* argv[])
{
	nit::embedded::core::application application;

	application.init();

	for(;;) {
		application.poll();
	}

	return EXIT_SUCCESS;
}