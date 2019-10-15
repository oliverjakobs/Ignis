#include "Ignis/Ignis.h"

#include "Ignis/Advanced/ComputeShader.h"

#include <GLFW/glfw3.h>

#include <spdlog/fmt/fmt.h>

#include <glm/gtc/random.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace ignis;

// settings
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

const glm::vec2 SCREEN_CENTER = { WIDTH / 2.0f, HEIGHT / 2.0f };

glm::mat4 SCREEN_MAT = glm::ortho(0.0f, (float)WIDTH, (float)HEIGHT, 0.0f);

// mouse input
glm::vec2 mousePos = SCREEN_CENTER;
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

	VertexArray vao;

	// position
	ArrayBuffer bufferPosition;
	bufferPosition.BufferData(PARTICLE_COUNT * sizeof(glm::vec4), NULL, GL_DYNAMIC_COPY);

	glm::vec4* positions = (glm::vec4*)glMapBufferRange(GL_ARRAY_BUFFER, 0, PARTICLE_COUNT * sizeof(glm::vec4), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

	for (int i = 0; i < PARTICLE_COUNT; i++)
	{
		positions[i] = glm::vec4(glm::diskRand(200.0f) + mousePos, 0.0f, 1.0f);
	}

	glUnmapBuffer(GL_ARRAY_BUFFER);

	bufferPosition.VertexAttribPointer(0, 4, 0, NULL);

	TextureBuffer texturePosition(GL_RGBA32F, bufferPosition.Name);

	// velocity
	ArrayBuffer bufferVelocity;
	bufferVelocity.BufferData(PARTICLE_COUNT * sizeof(glm::vec4), NULL, GL_DYNAMIC_COPY);

	glm::vec4* velocities = (glm::vec4*)glMapBufferRange(GL_ARRAY_BUFFER, 0, PARTICLE_COUNT * sizeof(glm::vec4), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

	for (int i = 0; i < PARTICLE_COUNT; i++)
	{
		velocities[i] = glm::vec4();
	}

	glUnmapBuffer(GL_ARRAY_BUFFER);

	TextureBuffer textureVelocity(GL_RGBA32F, bufferVelocity.Name);

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
		compShader.SetUniform2f("attractor", SCREEN_CENTER);

		texturePosition.BindImageTexture(0, GL_READ_WRITE);
		textureVelocity.BindImageTexture(1, GL_READ_WRITE);

		compShader.Dispatch(PARTICLE_GROUP_COUNT, 1, 1);

		glClear(GL_COLOR_BUFFER_BIT);

		glm::mat4 projection = glm::ortho(0.0f, (float)WIDTH, 0.0f, (float)HEIGHT);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 model = glm::mat4(1.0f);

		shader.Use();
		shader.SetUniformMat4("mvp", projection * view * model);

		vao.Bind();

		glPointSize(particleSize);
		glDrawArrays(GL_POINTS, 0, PARTICLE_COUNT);

		RenderText(fmt::format("FPS: {0}", timer.FPS), 0.0f, 32.0f, font, SCREEN_MAT, fontShader);
		RenderText(fmt::format("Particles: {0}", PARTICLE_COUNT), 0.0f, 64.0f, font, SCREEN_MAT, fontShader);
		RenderText(fmt::format("Particle size: {0}", particleSize), 0.0f, 96.0f, font, SCREEN_MAT, fontShader);

		glfwSwapBuffers(window);
		glfwPollEvents();

		timer.End((float)glfwGetTime());
	}
	
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}