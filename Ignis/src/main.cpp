#include "Ignis/Ignis.h"
#include "Ignis/Framebuffer.h"

#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

enum DemoProgram
{
	DEMO_TEXTURE,
	DEMO_INSTANCED,
	DEMO_ALPHA,
	DEMO_FRAMEBUFFER,
	DEMO_MODEL
};

int main()
{
	Ignis ignis;

	// GLFW initialization
	if (glfwInit() == GLFW_FALSE)
	{
		DEBUG_ERROR("[GLFW] Failed to initialize GLFW");
		glfwTerminate();
		return -1;
	}

	DEBUG_INFO("[GLFW] Initialized GLFW {0}", glfwGetVersionString());

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
		DEBUG_ERROR("[GLFW] Failed to create GLFW window");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	DEBUG_INFO("[GLFW] Window created.");

	// Set GLFW callbacks
	glfwSetErrorCallback([](int error, const char* desc)
	{
		DEBUG_ERROR("[GLFW] ({0}) {1}", error, desc);
	});

	bool debug = true;

	if (!ignis.LoadGL(glfwGetProcAddress, debug))
	{
		DEBUG_ERROR("[IGNIS] Failed to init renderer");
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

	vertices2 offsets;

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

	Shader* shader = new Shader("res/shaders/texture.vert", "res/shaders/texture.frag");
	Shader* instanced = new Shader("res/shaders/instanced.vert", "res/shaders/texture.frag");
	Shader* inverted = new Shader("res/shaders/texture.vert", "res/shaders/inverted.frag");
	Shader* grayscale = new Shader("res/shaders/texture.vert", "res/shaders/grayscale.frag");
	Shader* kernel = new Shader("res/shaders/texture.vert", "res/shaders/kernel.frag");

	Shader* test = new Shader("res/shaders/test.vert", "res/shaders/test.frag");

	Texture* texture = new Texture("res/textures/texture.png");

	Renderer renderer;

	DemoProgram prog = DEMO_MODEL;

	// --------------------------------------------------------------------
	FrameBuffer framebuffer = FrameBuffer(800, 600);
	
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glm::mat4 projection = glm::mat4(1.0f);
		glm::mat4 view = glm::ortho(0.0f, 8.0f, 0.0f, 6.0f);
		glm::mat4 model = glm::mat4(1.0f);

		vao.Bind();

		switch (prog)
		{
		case DEMO_TEXTURE:
			model = glm::translate(model, glm::vec3(2.0f, 1.0f, 1.0f));
			model = glm::scale(model, glm::vec3(4.0f, 4.0f, 1.0f));

			renderer.RenderTexture(texture, projection, view, model, test, { 0, 1, 2, 2, 3, 0 });
			break;
		case DEMO_INSTANCED:
			renderer.RenderTextureInstanced(texture, 48, projection, view, model, instanced, { 0, 1, 2, 2, 3, 0 });
			break;
		case DEMO_ALPHA:
			break;
		case DEMO_FRAMEBUFFER:
			framebuffer.Bind();
			//glEnable(GL_DEPTH_TEST);

			// make sure we clear the framebuffer's content
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			model = glm::translate(model, glm::vec3(2.0f, 1.0f, 1.0f));
			model = glm::scale(model, glm::vec3(4.0f, 4.0f, 1.0f));

			renderer.RenderTexture(texture, projection, view, model, shader, { 0, 1, 2, 2, 3, 0 });

			framebuffer.Unbind();
			//glDisable(GL_DEPTH_TEST); 

			framebuffer.VAO().Bind();
			renderer.RenderTexture(framebuffer.Texture(), glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f), kernel, { 0, 1, 2, 2, 3, 0 });
			break;
		case DEMO_MODEL:

			break;
		default:
			break;
		}

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