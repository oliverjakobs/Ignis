#include "Ignis/Renderer.h"
#include "Ignis/Utility.h"

#include <GLFW/glfw3.h>

int main()
{

	// GLFW initialization
	if (glfwInit() == GLFW_FALSE)
	{
		DEBUG_MESSAGE("[GLFW] Failed to initialize GLFW");
		glfwTerminate();
		return -1;
	}

	DEBUG_MESSAGE("[GLFW] Initialized GLFW " <<  glfwGetVersionString());

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef _DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

	// creating the window
	GLFWwindow* window = glfwCreateWindow(800, 600, "Ignis", nullptr, nullptr);
	if (window == nullptr)
	{
		DEBUG_MESSAGE("[GLFW] Failed to create GLFW window");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	//glfwSetWindowUserPointer(m_window, this);

	DEBUG_MESSAGE("[GLFW] Window created.");

	// Set GLFW callbacks
	glfwSetErrorCallback([](int error, const char* desc)
	{
		DEBUG_MESSAGE("[GLFW] (" << error << ") " << desc);
	});

	bool debug = true;

	Renderer renderer;
	if (!renderer.Init(glfwGetProcAddress, debug))
	{
		DEBUG_MESSAGE("[IGNIS] Failed to init renderer");
		glfwTerminate();
		return -1;
	}

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
	}
	

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}