#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>	//Graphics Library Framework (manage windows, OpenGL contexts and input)

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <vulkan/vulkan.h>

#include <iostream>
#include <stdexcept>
#include <functional>

const int WIDTH = 800;
const int HEIGHT = 600;

class VulkanBase {
public:
	void Run();

private:
	GLFWwindow * window;	//reference to the GLFW window
	VkInstance instance;	//handler to the instance

	void InitWindow();
	void InitVulkan();
	void CreateInstance();
	void MainLoop();
	void Cleanup();
};