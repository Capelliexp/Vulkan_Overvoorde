#pragma once
#include <GLFW/glfw3.h>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <vulkan/vulkan.h>

#include "VulkanBase.hpp"
#include "Callback.h"

#include <iostream>
#include <stdexcept>
#include <functional>
#include <vector>
#include <cstring>

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
	SetupDebugCallback();
}

void VulkanBase::CreateInstance() {
	std::cout << "CreateInstance()" << std::endl;

	if (enableValidationLayers && !CheckValidationLayerSupport()) {
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
	//createInfo.enabledExtensionCount = ;		//see below (Extensions/Msg Callback)
	//createInfo.ppEnabledExtensionNames = ;	//see below (Extensions/Msg Callback)

	//---------------- EXTENSIONS / MESSAGE CALLBACK ----------------
	//return the extensions we need to interface with the window system and pass to a struct

	auto extensions = GetRequiredExtensions();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

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

bool VulkanBase::CheckValidationLayerSupport() {
	std::cout << "CheckValidationLayerSupport()" << std::endl;

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

std::vector<const char*> VulkanBase::GetRequiredExtensions() {
	//enabling relaying of debug messages back to our program
	std::cout << "GetRequiredExtensions()" << std::endl;

	std::vector<const char*> extensions;

	unsigned int glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	for (unsigned int i = 0; i < glfwExtensionCount; i++) {
		extensions.push_back(glfwExtensions[i]);
	}

	if (enableValidationLayers) {
		extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	}

	return extensions;
}

void VulkanBase::SetupDebugCallback() {
	std::cout << "SetupDebugCallback()" << std::endl;
	if (!enableValidationLayers) return;

	VkDebugReportCallbackCreateInfoEXT createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
	createInfo.pfnCallback = debugCallback;

	if (CreateDebugReportCallbackEXT(instance, &createInfo, nullptr, &callback) != VK_SUCCESS) {
		throw std::runtime_error("failed to set up debug callback!");
		std::cout << "   failed to set up debug callback!" << std::endl;
	}
}

void VulkanBase::MainLoop() {
	std::cout << "MainLoop()" << std::endl;

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}
}

void VulkanBase::Cleanup() {
	std::cout << "Cleanup()" << std::endl;

	DestroyDebugReportCallbackEXT(instance, callback, nullptr);
	vkDestroyInstance(instance, nullptr);
	glfwDestroyWindow(window);
	glfwTerminate();
}