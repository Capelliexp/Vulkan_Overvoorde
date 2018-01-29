#include "VulkanBase.hpp"

void VulkanBase::Run() {
	InitWindow();
	InitVulkan();
	MainLoop();
	Cleanup();
}

void VulkanBase::InitWindow() {
	std::cout << "InitWindow()" << std::endl;


	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);	// don't create an OpenGL context
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);		//don't make the window resizeable

	window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
}

void VulkanBase::InitVulkan() {
	std::cout << "InitVulkan()" << std::endl;

	CreateInstance();

}

void VulkanBase::CreateInstance() {
	std::cout << "createInstance()" << std::endl;

	if (enableValidationLayers && !checkValidationLayerSupport()) {
		throw std::runtime_error("validation layers requested, but not available!");
	}

	VkApplicationInfo appInfo = {};	//some information about our application
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	//appInfo.pNext = ;
	appInfo.pApplicationName = "Hello Triangle";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo = {};	//which global extensions and validation layers we want to use
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	//createInfo.pNext = ;
	//createInfo.flags = ;
	createInfo.pApplicationInfo = &appInfo;
	//createInfo.enabledLayerCount = 0;			//see below (Validation)
	//createInfo.ppEnabledLayerNames = ;		//see below (Validation)
	//createInfo.enabledExtensionCount = ;		//see below (Extensions)
	//createInfo.ppEnabledExtensionNames = ;	//see below (Extensions)

	//---------------- EXTENSIONS ----------------
	//return the extensions we need to interface with the window system and pass to a struct

	unsigned int glfwExtensionCount = 0;
	const char** glfwExtensions;

	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	createInfo.enabledExtensionCount = glfwExtensionCount;
	createInfo.ppEnabledExtensionNames = glfwExtensions;

	//---------------- VALIDATION ----------------
	//checks if validation layers are enabled and edits the createInfo struct

	if (enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else {
		createInfo.enabledLayerCount = 0;
	}

	//----------------

	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)	//create instance
		throw std::runtime_error("failed to create instance");
}

bool VulkanBase::checkValidationLayerSupport() {
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : validationLayers) {
		bool layerFound = false;
		for (const auto& layerProperties : availableLayers) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}
		if (!layerFound) {
			return false;
		}
	}
	return true;
}

void VulkanBase::MainLoop() {
	std::cout << "MainLoop()" << std::endl;

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}
}

void VulkanBase::Cleanup() {
	std::cout << "Cleanup()" << std::endl;

	glfwDestroyWindow(window);
	glfwTerminate();
}