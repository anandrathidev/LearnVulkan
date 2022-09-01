#include "lve_window.h"
#include <stdexcept>
namespace lve {

	LveWindow::LveWindow(int width, int height, std::string name) :_width(width), _height(height), _windowname(name)
	{
		initLveWindow();
	}

	LveWindow::~LveWindow() 
	{
		if(_window)
			glfwDestroyWindow(_window);
		glfwTerminate();
		
	}
	

	void LveWindow::createWindowSurface(VkInstance& instance, VkSurfaceKHR* surface_)
	{
		if (glfwCreateWindowSurface(instance, _window, nullptr, surface_ )!=VK_SUCCESS)
		{
			throw std::runtime_error("failed to create window surface");
		}
	}

	void LveWindow::framebufferResizeCallback(GLFWwindow* window, int width, int height)
	{
		auto lveWindow = reinterpret_cast<LveWindow*>(glfwGetWindowUserPointer(window));
		lveWindow->_frameBufferresized = true;
		lveWindow->_width = width;
		lveWindow->_height = height;
	}


	void LveWindow::initLveWindow()
	{
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		//glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		_window = glfwCreateWindow(_width, _height, _windowname.c_str(), nullptr, nullptr);
		//_window->reset(glfwCreateWindow(_width, _height, _windowname.c_str(), nullptr, nullptr));
		glfwSetWindowUserPointer(_window, this);
		glfwSetFramebufferSizeCallback(_window, LveWindow::framebufferResizeCallback);


	}

}