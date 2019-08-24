#include "Ignis/Ignis.h"
#include "Ignis/Utility.h"

#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
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

	DEBUG_MESSAGE("[GLFW] Window created.");

	// Set GLFW callbacks
	glfwSetErrorCallback([](int error, const char* desc)
	{
		DEBUG_MESSAGE("[GLFW] (" << error << ") " << desc);
	});

	bool debug = true;

	Ignis ignis;
	if (!ignis.LoadGL(glfwGetProcAddress, debug))
	{
		DEBUG_MESSAGE("[IGNIS] Failed to init renderer");
		glfwTerminate();
		return -1;
	}

	ignis.EnableBlend(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	ignis.SetClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	VAO vao;

	GLfloat vertices[] =
	{
		0.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f
	};

	std::vector<glm::vec2> offsets;

	for (float y = 0.0f; y < 6.0f; y++)
	{
		for (float x = 0.0f; x < 8.0f; x++)
		{
			offsets.push_back(glm::vec2(x, y));
		}
	}

	vao.Bind();

	vao.GenVertexBuffer();

	vao.SetVertexBufferData(sizeof(vertices), vertices);
	vao.SetVertexAttribPointer(0, 2, 4, 0);
	vao.SetVertexAttribPointer(1, 2, 4, 2);

	vao.GenVertexBuffer();
	vao.SetVertexBufferData(sizeof(glm::vec2) * offsets.size(), &offsets[0]);
	vao.SetVertexAttribPointer(2, 2, 2, 0);
	vao.SetVertexAttribDivisor(2, 1);

	vao.UnbindVertexBuffer();

	Shader* shader = new Shader("res/shaders/instanced.vert", "res/shaders/texture.frag");
	Texture* texture = new Texture("res/textures/texture.png");

	Renderer renderer;

	glm::mat4 view = glm::ortho(0.0f, 8.0f, 0.0f, 6.0f);
	
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		vao.Bind();

		renderer.RenderTextureInstanced(texture, 8 * 6, glm::mat4(1.0f), view, glm::mat4(1.0f), shader);

		vao.Unbind();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	SAFE_DELETE(shader);
	SAFE_DELETE(texture);
	
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}