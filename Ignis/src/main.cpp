#include "Ignis/Ignis.h"

#include <GLFW/glfw3.h>

#include <spdlog/fmt/fmt.h>

#include <glm/gtc/random.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Utility/Utility.h"

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

GLuint CreateComputeShader(const char* src)
{
	GLuint program  = glCreateProgram();

	GLuint shader = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);

	// check shader errors
	GLint params = -1;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &params);
	if (GL_TRUE != params) 
	{
		fprintf(stderr, "ERROR: shader %u did not compile\n", shader);
		int max_length = 4096;
		int actual_length = 0;
		char slog[4096];
		glGetShaderInfoLog(shader, max_length, &actual_length, slog);
		fprintf(stderr, "shader info log for GL index %u\n%s\n", shader, slog);
		return 0;
	}

	glAttachShader(program, shader);
	glLinkProgram(program);

	//(check_program_errors(program)); // code moved to gl_utils.cpp
	params = -1;
	glGetProgramiv(program, GL_LINK_STATUS, &params);
	if (GL_TRUE != params) 
	{
		fprintf(stderr, "ERROR: program %u did not link\n", program);
		int max_length = 4096;
		int actual_length = 0;
		char plog[4096];
		glGetProgramInfoLog(program, max_length, &actual_length, plog);
		fprintf(stderr, "program info log for GL index %u\n%s\n", program, plog);
		glDeleteShader(shader);
		glDeleteProgram(program);
		return 0;
	}
	glDeleteShader(shader);

	return program;
}

#if 1
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

	FrameCounter frames;

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

	GLuint compShader = CreateComputeShader(ReadFile("res/shaders/particles.comp").c_str());
	glUseProgram(compShader);

	GLuint l_mousePos = glGetUniformLocation(compShader, "mousePos");
	GLuint l_deltaTime = glGetUniformLocation(compShader, "deltaTime");

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
		frames.Start((float)glfwGetTime());

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		// update particleSize
		particleSize += mouseScroll;
		mouseScroll = 0.0f;
		if (particleSize < 1.0f)
			particleSize = 1.0f;

		glUseProgram(compShader);
		glUniform1f(l_deltaTime, frames.DeltaTime);
		glUniform2fv(l_mousePos, 1, &mousePos[0]);

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

		RenderText(fmt::format("FPS: {0}", frames.FPS), 0.0f, 32.0f, font, SCREEN_MAT, fontShader);
		RenderText(fmt::format("Particles: {0}", PARTICLE_COUNT), 0.0f, 64.0f, font, SCREEN_MAT, fontShader);
		RenderText(fmt::format("Particle size: {0}", particleSize), 0.0f, 96.0f, font, SCREEN_MAT, fontShader);

		glfwSwapBuffers(window);
		glfwPollEvents();

		frames.End((float)glfwGetTime());
	}

	glDeleteBuffers(1, &bufferPosition);
	glDeleteBuffers(1, &bufferVelocity);

	glDeleteTextures(1, &texturePosition);
	glDeleteTextures(1, &textureVelocity);

	glDeleteVertexArrays(1, &vao);

	glDeleteProgram(compShader);
	
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
#endif
