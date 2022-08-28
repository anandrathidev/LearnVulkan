#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>
#include <memory>
namespace lve {
	/*
	* This class implements wrapper to Windows application window and pols for it events via GLFWwindow
	* When onCLose event is recived closes the app .  
	*/
	class LveWindow {
	public:
		LveWindow(int width, int height, std::string name);
		LveWindow(const LveWindow&) = delete;
		const LveWindow& operator=(const LveWindow&) = delete;
		bool shouldClose() { return glfwWindowShouldClose(_window); }
		VkExtent2D getExtent() { return { static_cast<uint32_t>(_width), static_cast<uint32_t>(_height) }; }
		void createWindowSurface(VkInstance& instance, VkSurfaceKHR* surface_);
		~LveWindow();
	private:
		int _width;
		int _height;
		std::string _windowname;

		void initLveWindow();
		//std::unique_ptr<GLFWwindow, void(GLFWwindow*)>  _window;
		GLFWwindow* _window = 0;
	};


} // namespace lve 