#include "Ignis/Ignis.h"

#include "Ignis/Framebuffer.h"
#include "Ignis/Mesh.h"
#include "Ignis/Camera.h"

#include <GLFW/glfw3.h>

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

float mouseOffsetX = 0.0f;
float mouseOffsetY = 0.0f;

bool firstMouse = true;

void DemoTexture(GLFWwindow* window)
{
	VAO vao;

	GLfloat vertices[] =
	{
		0.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f
	};

	vao.Bind();

	vao.GenVertexBuffer();

	vao.SetVertexBufferData(sizeof(vertices), vertices);
	vao.SetVertexAttribPointer(0, 2, 4, 0);
	vao.SetVertexAttribPointer(1, 2, 4, 2);

	vao.UnbindVertexBuffer();

	Shader shader = Shader("res/shaders/texture.vert", "res/shaders/texture.frag");

	Texture texture = Texture("res/textures/texture.png");

	Renderer renderer;

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glm::mat4 projection = glm::mat4(1.0f);
		glm::mat4 view = glm::ortho(0.0f, 8.0f, 0.0f, 6.0f);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 1.0f));

		vao.Bind();
		renderer.RenderTexture(texture, projection, view, model, shader, { 0, 1, 2, 2, 3, 0 });

		vao.Unbind();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void DemoInstanced(GLFWwindow* window)
{
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

	Shader instanced = Shader("res/shaders/instanced.vert", "res/shaders/texture.frag");

	Texture texture = Texture("res/textures/texture.png");

	Renderer renderer;

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glm::mat4 projection = glm::mat4(1.0f);
		glm::mat4 view = glm::ortho(0.0f, 8.0f, 0.0f, 6.0f);
		glm::mat4 model = glm::mat4(1.0f);

		vao.Bind();

		renderer.RenderTextureInstanced(texture, 48, projection, view, model, instanced, { 0, 1, 2, 2, 3, 0 });

		vao.Unbind();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void DemoFramebuffer(GLFWwindow* window)
{
	VAO vao;

	GLfloat vertices[] =
	{
		0.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f
	};

	vao.Bind();

	vao.GenVertexBuffer();

	vao.SetVertexBufferData(sizeof(vertices), vertices);
	vao.SetVertexAttribPointer(0, 2, 4, 0);
	vao.SetVertexAttribPointer(1, 2, 4, 2);

	vao.UnbindVertexBuffer();

	Shader shader = Shader("res/shaders/texture.vert", "res/shaders/texture.frag");
	Shader inverted = Shader("res/shaders/texture.vert", "res/shaders/inverted.frag");
	Shader grayscale = Shader("res/shaders/texture.vert", "res/shaders/grayscale.frag");
	Shader kernel = Shader("res/shaders/texture.vert", "res/shaders/kernel.frag");

	Texture texture = Texture("res/textures/texture.png");

	Renderer renderer;

	FrameBuffer framebuffer = FrameBuffer(800, 600);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glm::mat4 projection = glm::mat4(1.0f);
		glm::mat4 view = glm::ortho(0.0f, 8.0f, 0.0f, 6.0f);
		glm::mat4 model = glm::mat4(1.0f);

		vao.Bind();

		framebuffer.Bind();
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		model = glm::translate(model, glm::vec3(2.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 1.0f));

		renderer.RenderTexture(texture, projection, view, model, shader, { 0, 1, 2, 2, 3, 0 });

		framebuffer.Unbind();

		framebuffer.BindVAO();
		renderer.RenderTexture(framebuffer.Texture(), glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f), kernel, { 0, 1, 2, 2, 3, 0 });

		vao.Unbind();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void DemoModel(GLFWwindow* window)
{
	float cameraSpeed = 2.5f;
	float cameraSensitivity = 0.1f;

	glEnable(GL_DEPTH_TEST);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xPos, double yPos)
	{
		if (firstMouse)
		{
			lastX = xPos;
			lastY = yPos;
			firstMouse = false;
		}

		mouseOffsetX = xPos - lastX;
		mouseOffsetY = lastY - yPos; // reversed since y-coordinates go from bottom to top

		lastX = xPos;
		lastY = yPos;
	});

	Renderer renderer;

	// timing
	float deltaTime = 0.0f;	// time between current frame and last frame
	float lastFrame = 0.0f;

	Shader shader = Shader("res/shaders/model.vert", "res/shaders/model.frag");

	Mesh mesh = Mesh("res/models/barrel.obj");
	Texture texture = Texture("res/models/barrel.png");
	
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		float velocity = cameraSpeed * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera.Move(camera.Front * velocity);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.Move(-camera.Front * velocity);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera.Move(-camera.Right * velocity);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera.Move(camera.Right * velocity);

		camera.YawPitch(mouseOffsetX * cameraSensitivity, mouseOffsetY * cameraSensitivity);
		mouseOffsetX = 0.0f;
		mouseOffsetY = 0.0f;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glm::mat4 projection = glm::perspective(70.0f, 800.0f / 600.0f, 0.1f, 100.0f);
		glm::mat4 view = camera.View();
		glm::mat4 model = glm::mat4(1.0f);

		shader.Use();

		shader.SetUniformMat4("projection", projection);
		shader.SetUniformMat4("view", view);
		shader.SetUniformMat4("model", model);

		texture.Bind();
		mesh.Draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

enum DemoProgram
{
	DEMO_TEXTURE,
	DEMO_INSTANCED,
	DEMO_FRAMEBUFFER,
	DEMO_MODEL,
	DEMO_FONT,
	DEMO_ALPHA
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
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Ignis", nullptr, nullptr);
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
		DEBUG_ERROR("[IGNIS] Failed to load OpenGL");
		glfwTerminate();
		return -1;
	}

	ignis.EnableBlend(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	ignis.SetClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	DemoProgram prog = DEMO_MODEL;

	switch (prog)
	{
	case DEMO_TEXTURE:
		DemoTexture(window);
		break;
	case DEMO_INSTANCED:
		DemoInstanced(window);
		break;
	case DEMO_FRAMEBUFFER:
		DemoFramebuffer(window);
		break;
	case DEMO_MODEL:
		DemoModel(window);
		break;
	default:
		break;
	}
	
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
