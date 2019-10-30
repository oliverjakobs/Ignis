#include "Ignis/Ignis.h"

#include "Ignis/Advanced/ComputeShader.h"

#include <GLFW/glfw3.h>

#include <glm/gtc/random.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ImGuiBinding/ImGuiRenderer.h"

#include "Ignis/Renderer/RenderState.h"

#include "Obelisk/Obelisk.h"

using namespace ignis;

// settings
const uint WIDTH = 800;
const uint HEIGHT = 600;

const glm::vec2 SCREEN_CENTER = { WIDTH / 2.0f, HEIGHT / 2.0f };

// mouse input
glm::vec2 mousePos = SCREEN_CENTER;
float mouseScroll = 0.0f;

const int PARTICLE_GROUP_SIZE = 64;
const int PARTICLE_GROUP_COUNT = 128;
const int PARTICLE_COUNT = (PARTICLE_GROUP_SIZE * PARTICLE_GROUP_COUNT);

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

	Shader shader = Shader("res/shaders/particles.vert", "res/shaders/particles.frag");
	ComputeShader compShader = ComputeShader("res/shaders/particles.comp");

	VertexArray vao;

	// position
	ArrayBuffer bufferPosition;
	bufferPosition.BufferData(PARTICLE_COUNT * sizeof(glm::vec4), nullptr, GL_DYNAMIC_COPY);
	bufferPosition.VertexAttribPointer(0, 4, 0, 0);

	glm::vec4* positions = bufferPosition.MapBufferRange<glm::vec4>(0, PARTICLE_COUNT, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

	for (int i = 0; i < PARTICLE_COUNT; i++)
		positions[i] = glm::vec4(SCREEN_CENTER, (float)i / (float)PARTICLE_COUNT, 0.0f);

	bufferPosition.UnmapBuffer();

	// velocity
	ArrayBuffer bufferVelocity;
	bufferVelocity.BufferData(PARTICLE_COUNT * sizeof(glm::vec4), nullptr, GL_DYNAMIC_COPY);

	glm::vec4* velocities = bufferVelocity.MapBufferRange<glm::vec4>(0, PARTICLE_COUNT, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

	for (int i = 0; i < PARTICLE_COUNT; i++)
		velocities[i] = glm::vec4(glm::diskRand(1.0f), 1.0f, 0.0f);

	bufferVelocity.UnmapBuffer();

	// textures buffer
	TextureBuffer texturePosition(GL_RGBA32F, bufferPosition.Name);
	TextureBuffer textureVelocity(GL_RGBA32F, bufferVelocity.Name);

	// config 
	float particleSize = 2.0f;
	float particleRadius = 100.0f;
	color particleColor = WHITE;
	
	while (!glfwWindowShouldClose(window))
	{
		timer.Start((float)glfwGetTime());

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		compShader.Use();
		compShader.SetUniform1f("deltaTime", timer.DeltaTime);
		compShader.SetUniform1f("particleRadius", particleRadius);
		compShader.SetUniform2f("mousePos", mousePos);

		texturePosition.BindImageTexture(0, GL_READ_WRITE);
		textureVelocity.BindImageTexture(1, GL_READ_WRITE);

		compShader.Dispatch(PARTICLE_GROUP_COUNT, 1, 1, GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

		glClear(GL_COLOR_BUFFER_BIT);

		glm::mat4 projection = glm::ortho(0.0f, (float)WIDTH, 0.0f, (float)HEIGHT);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 model = glm::mat4(1.0f);

		shader.Use();
		shader.SetUniformMat4("mvp", projection * view * model);
		shader.SetUniform4f("particleColor", particleColor);

		vao.Bind();

		glPointSize(particleSize);
		glDrawArrays(GL_POINTS, 0, PARTICLE_COUNT);

		// gui
		ImGuiRenderer::Begin();

		ImGui::Begin("Settings");
		ImGui::Text("Particles (%d)", PARTICLE_COUNT);
		ImGui::SliderFloat("Size", &particleSize, 1, 10, "%.1f");
		ImGui::SliderFloat("Radius", &particleRadius, 10, 200, "%.1f");
		ImGui::ColorEdit4("Color", &particleColor[0]);
		ImGui::End();

		ImGuiRenderer::End();

		// debug info
		fontShader.Use();
		fontShader.SetUniformMat4("projection", Ignis::ScreenMat);

		FontRenderer::RenderText(font, fmt::format("FPS: {0}", timer.FPS), 0.0f, 32.0f);

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
	obelisk::Logger::SetFormat("[%^%l%$] %v");
	obelisk::Logger::SetLevel(obelisk::LogLevel::Trace);

	// ingis initialization
	if (!Ignis::Init(width, height))
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

	OBELISK_INFO("[GLFW] Initialized GLFW {0}", glfwGetVersionString());

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
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

	OBELISK_INFO("[GLFW] Window created.");

	// Set GLFW callbacks
	glfwSetErrorCallback([](int error, const char* desc)
	{
		OBELISK_ERROR("[GLFW] ({0}) {1}", error, desc);
	});

	bool debug = true;

	if (!Ignis::LoadGL(debug))
	{
		OBELISK_ERROR("[IGNIS] Failed to load OpenGL");
		glfwTerminate();
		return nullptr;
	}

	OBELISK_INFO("[OpenGL] Version: {0}", glGetString(GL_VERSION));
	OBELISK_INFO("[OpenGL] Vendor: {0}", glGetString(GL_VENDOR));
	OBELISK_INFO("[OpenGL] Renderer: {0}", glGetString(GL_RENDERER));
	OBELISK_INFO("[OpenGL] GLSL Version: {0}", glGetString(GL_SHADING_LANGUAGE_VERSION));

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