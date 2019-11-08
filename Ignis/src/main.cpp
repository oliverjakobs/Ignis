#include "Ignis/Ignis.h"

#include <GLFW/glfw3.h>

#include <glm/gtc/random.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp> 

#include "Ignis/Advanced/ComputeShader.h"

#include "Obelisk/Obelisk.h"
#include "ImGuiBinding/ImGuiRenderer.h"

#include "Ignis/Camera/FpsCamera.h"

using namespace ignis;

// settings
uint WIDTH = 1280;
uint HEIGHT = 720;

glm::vec2 SCREEN_CENTER = { WIDTH / 2.0f, HEIGHT / 2.0f };
float ASPECT_RATIO = (float)WIDTH / (float)HEIGHT;

// mouse input
glm::vec2 mousePos = SCREEN_CENTER;
glm::vec2 mouseOffset = glm::vec2();
float mouseScroll = 0.0f;

bool firstMouse = true;

float cameraSpeed = 5.0f;
float cameraRotation = 180.0f;
float cameraZoom = 1.0f;

OrthographicCamera camera = OrthographicCamera(-ASPECT_RATIO * cameraZoom, ASPECT_RATIO* cameraZoom, -cameraZoom, cameraZoom);

GLFWwindow* Init(const char* title, uint width, uint height);

int main()
{
	GLFWwindow* window = Init("Ignis", WIDTH, HEIGHT);

	if (!window) return -1;

	// imgui
	ImGuiRenderer::Init(window);

	obelisk::Timer timer;

	Font font = Font("res/fonts/OpenSans.ttf", 32.0f);
	Shader fontShader = Shader("res/shaders/font.vert", "res/shaders/font.frag");

	// configure render state
	RenderState renderState;
	renderState.SetBlend(true, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	renderState.SetDepthTest(true);
	renderState.SetCullFace(true);

	glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height)
	{
		WIDTH = width;
		HEIGHT = height;

		glm::vec2 SCREEN_CENTER = { width / 2.0f, height / 2.0f };

		ASPECT_RATIO = (float)width / (float)height;
		camera.SetProjection(-ASPECT_RATIO * cameraZoom, ASPECT_RATIO * cameraZoom, -cameraZoom, cameraZoom);

		ignisViewport(0, 0, width, height);
	});

	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xPos, double yPos)
	{
		if (firstMouse)
		{
			mousePos = glm::vec2((float)xPos, (float)yPos);
			firstMouse = false;
		}

		mouseOffset = glm::vec2((float)xPos - mousePos.x, mousePos.y - (float)yPos);
		mousePos = glm::vec2((float)xPos, (float)yPos);
	});

	glfwSetScrollCallback(window, [](GLFWwindow* window, double xOffset, double yOffset)
	{
		cameraZoom -= yOffset * 0.25f;
		cameraZoom = max(cameraZoom, 0.25f);
		camera.SetProjection(-ASPECT_RATIO * cameraZoom, ASPECT_RATIO * cameraZoom, -cameraZoom, cameraZoom);
	});
	
	Renderer2D::Init();

	std::shared_ptr<Texture> texture = std::make_shared<Texture>("res/textures/checkerboard.png");

	while (!glfwWindowShouldClose(window))
	{
		timer.Start((float)glfwGetTime());

		// input
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		// camera movement
		glm::vec3 position = camera.GetPosition();
		float rotation = camera.GetRotation();

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			position.x -= cos(glm::radians(rotation)) * cameraSpeed * timer.DeltaTime;
			position.y -= sin(glm::radians(rotation)) * cameraSpeed * timer.DeltaTime;
		}
		else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			position.x += cos(glm::radians(rotation)) * cameraSpeed * timer.DeltaTime;
			position.y += sin(glm::radians(rotation)) * cameraSpeed * timer.DeltaTime;
		}

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			position.x += -sin(glm::radians(rotation)) * cameraSpeed * timer.DeltaTime;
			position.y += cos(glm::radians(rotation)) * cameraSpeed * timer.DeltaTime;
		}
		else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			position.x -= -sin(glm::radians(rotation)) * cameraSpeed * timer.DeltaTime;
			position.y -= cos(glm::radians(rotation)) * cameraSpeed * timer.DeltaTime;
		}

		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			rotation += cameraRotation * timer.DeltaTime;
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
			rotation -= cameraRotation * timer.DeltaTime;

		if (rotation > 180.0f)
			rotation -= 360.0f;
		else if (rotation <= -180.0f)
			rotation += 360.0f;

		camera.SetPosition(position);
		camera.SetRotation(rotation);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Renderer2D::BeginScene(camera.GetViewProjection());

		Renderer2D::RenderQuad({ -1.0f, 0.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Renderer2D::RenderQuad({ 0.5f, -0.5f, 0.0f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		Renderer2D::RenderQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, texture);

		Renderer2D::EndScene();

		// gui
		// ImGuiRenderer::Begin();
		// ImGuiRenderer::End();

		// debug info
		fontShader.Use();
		fontShader.SetUniformMat4("projection", ignisScreenMat());

		FontRenderer::RenderText(font, obelisk::format("FPS: %d", timer.FPS), 0.0f, 32.0f);

		glfwSwapBuffers(window);
		glfwPollEvents();

		timer.End((float)glfwGetTime());
	}
	
	ImGuiRenderer::Quit();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

GLFWwindow* Init(const char* title, uint width, uint height)
{
	// ingis initialization
	if (!ignisInit(width, height))
	{
		OBELISK_ERROR("[Ignis] Failed to initialize Ignis");
		return nullptr;
	}
	
	ignisSetErrorCallback([](ignisErrorLevel level, const std::string& desc)
	{
		switch (level)
		{
		case ignisErrorLevel::Warn:		OBELISK_WARN(desc.c_str()); break;
		case ignisErrorLevel::Error:	OBELISK_ERROR(desc.c_str()); break;
		case ignisErrorLevel::Critical:	OBELISK_CRITICAL(desc.c_str()); break;
		}
	});

	// GLFW initialization
	if (glfwInit() == GLFW_FALSE)
	{
		OBELISK_ERROR("[GLFW] Failed to initialize GLFW");
		glfwTerminate();
		return nullptr;
	}

	OBELISK_INFO("[GLFW] Initialized GLFW %s", glfwGetVersionString());

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef _DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

	// creating the window
	GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (window == nullptr)
	{
		OBELISK_ERROR("[GLFW] Failed to create GLFW window");
		glfwTerminate();
		return nullptr;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	// Set GLFW callbacks
	glfwSetErrorCallback([](int error, const char* desc)
	{
		OBELISK_ERROR("[GLFW] (%d) %s", error, desc);
	});

	bool debug = true;

	if (!ignisLoadGL(debug))
	{
		OBELISK_ERROR("[IGNIS] Failed to load OpenGL");
		glfwTerminate();
		return nullptr;
	}

	OBELISK_INFO("[OpenGL] Version: %s", glGetString(GL_VERSION));
	OBELISK_INFO("[OpenGL] Vendor: %s", glGetString(GL_VENDOR));
	OBELISK_INFO("[OpenGL] Renderer: %s", glGetString(GL_RENDERER));
	OBELISK_INFO("[OpenGL] GLSL Version: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xPos, double yPos)
	{
		mousePos = { (float)xPos, HEIGHT - (float)yPos };
	});

	glfwSetScrollCallback(window, [](GLFWwindow* window, double xOffset, double yOffset)
	{
		mouseScroll = (float)yOffset;
	});

	return window;
}