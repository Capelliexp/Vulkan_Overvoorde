#include "VulkanBase.hpp"

#include <iostream>

int main() {
	VulkanBase App;

	try {
		App.Run();
	}
	catch (const std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	getchar();
	return EXIT_SUCCESS;
}