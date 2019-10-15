#include "Ignis/Ignis.h"

#include <GLFW/glfw3.h>

#include <spdlog/fmt/fmt.h>

#include <glm/gtc/random.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace ignis;

// settings
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

glm::mat4 SCREEN_MAT = glm::ortho(0.0f, (float)WIDTH, (float)HEIGHT, 0.0f);

// mouse input
glm::vec2 mousePos = { WIDTH / 2.0f, HEIGHT / 2.0f };
float mouseScroll = 0.0f;

const int PARTICLE_GROUP_SIZE = 64;
const int PARTICLE_GROUP_COUNT = 128;
const int PARTICLE_COUNT = (PARTICLE_GROUP_SIZE * PARTICLE_GROUP_COUNT);

const float MAX_PARTICLE_SIZE = 10.0f;

int main()
{
	// ingis initialization
	if (!InitIgnis())
	{
		DEBUG_ERROR("[Ignis] Failed to initialize Ignis");
		return -1;
	}

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
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ignis", nullptr, nullptr);
	if (window == nullptr)
	{
		DEBUG_ERROR("[GLFW] Failed to create GLFW window");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	DEBUG_INFO("[GLFW] Window created.");

	// Set GLFW callbacks
	glfwSetErrorCallback([](int error, const char* desc)
	{
		DEBUG_ERROR("[GLFW] ({0}) {1}", error, desc);
	});

	bool debug = true;

	if (!LoadGL(glfwGetProcAddress, debug))
	{
		DEBUG_ERROR("[IGNIS] Failed to load OpenGL");
		glfwTerminate();
		return -1;
	}

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	obelisk::Timer timer;

	Font font = Font("res/fonts/OpenSans.ttf", 32.0f);
	Shader fontShader = Shader("res/shaders/font.vert", "res/shaders/font.frag");

	// configure global opengl state
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xPos, double yPos)
	{
		mousePos = { (float)xPos, HEIGHT - (float)yPos };
	});

	glfwSetScrollCallback(window, [](GLFWwindow* window, double xOffset, double yOffset)
	{
		mouseScroll = (float)yOffset;
	});

	Shader shader = Shader("res/shaders/particles.vert", "res/shaders/particles.frag");
	ComputeShader compShader = ComputeShader("res/shaders/particles.comp");

	uint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// position
	uint bufferPosition;
	glGenBuffers(1, &bufferPosition);
	glBindBuffer(GL_ARRAY_BUFFER, bufferPosition);
	glBufferData(GL_ARRAY_BUFFER, PARTICLE_COUNT * sizeof(glm::vec4), NULL, GL_DYNAMIC_COPY);

	glm::vec4* positions = (glm::vec4*)glMapBufferRange(GL_ARRAY_BUFFER, 0, PARTICLE_COUNT * sizeof(glm::vec4), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

	for (int i = 0; i < PARTICLE_COUNT; i++)
	{
		positions[i] = glm::vec4(glm::diskRand(100.0f) + mousePos, 0.0f, 1.0f);
	}

	glUnmapBuffer(GL_ARRAY_BUFFER);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	uint texturePosition;
	glGenTextures(1, &texturePosition);

	glBindTexture(GL_TEXTURE_BUFFER, texturePosition);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, bufferPosition);

	// velocity
	uint bufferVelocity;
	glGenBuffers(1, &bufferVelocity);
	glBindBuffer(GL_ARRAY_BUFFER, bufferVelocity);
	glBufferData(GL_ARRAY_BUFFER, PARTICLE_COUNT * sizeof(glm::vec4), NULL, GL_DYNAMIC_COPY);

	glm::vec4* velocities = (glm::vec4*)glMapBufferRange(GL_ARRAY_BUFFER, 0, PARTICLE_COUNT * sizeof(glm::vec4), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

	for (int i = 0; i < PARTICLE_COUNT; i++)
	{
		velocities[i] = glm::vec4();
	}

	glUnmapBuffer(GL_ARRAY_BUFFER);

	uint textureVelocity;
	glGenTextures(1, &textureVelocity);

	glBindTexture(GL_TEXTURE_BUFFER, textureVelocity);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, textureVelocity);

	// config 
	float particleSize = 2.0f;

	while (!glfwWindowShouldClose(window))
	{
		timer.Start((float)glfwGetTime());

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		// update particleSize
		particleSize = glm::clamp(particleSize + mouseScroll, 1.0f, MAX_PARTICLE_SIZE);
		mouseScroll = 0.0f;

		compShader.Use();
		compShader.SetUniform1f("deltaTime", timer.DeltaTime);
		compShader.SetUniform2f("mousePos", mousePos);

		glBindImageTexture(0, texturePosition, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
		glBindImageTexture(1, textureVelocity, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
		glDispatchCompute(PARTICLE_GROUP_COUNT, 1, 1);

		// prevent sampling befor all writes to image are done
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

		glClear(GL_COLOR_BUFFER_BIT);

		glm::mat4 projection = glm::ortho(0.0f, (float)WIDTH, 0.0f, (float)HEIGHT);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 model = glm::mat4(1.0f);

		shader.Use();
		shader.SetUniformMat4("mvp", projection * view * model);

		glBindVertexArray(vao);
		//glBlendFunc(GL_ONE, GL_ONE);
		glPointSize(particleSize);
		glDrawArrays(GL_POINTS, 0, PARTICLE_COUNT);

		RenderText(fmt::format("FPS: {0}", timer.FPS), 0.0f, 32.0f, font, SCREEN_MAT, fontShader);
		RenderText(fmt::format("Particles: {0}", PARTICLE_COUNT), 0.0f, 64.0f, font, SCREEN_MAT, fontShader);
		RenderText(fmt::format("Particle size: {0}", particleSize), 0.0f, 96.0f, font, SCREEN_MAT, fontShader);

		glfwSwapBuffers(window);
		glfwPollEvents();

		timer.End((float)glfwGetTime());
	}

	glDeleteBuffers(1, &bufferPosition);
	glDeleteBuffers(1, &bufferVelocity);

	glDeleteTextures(1, &texturePosition);
	glDeleteTextures(1, &textureVelocity);

	glDeleteVertexArrays(1, &vao);
	
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}