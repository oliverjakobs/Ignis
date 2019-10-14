#include "Ignis/Ignis.h"

#include "Ignis/Framebuffer.h"
#include "Ignis/Camera.h"

#include <GLFW/glfw3.h>

#include <spdlog/fmt/fmt.h>

#include <glm/gtx/rotate_vector.hpp> 
#include <glm/gtc/random.hpp> 
#include <stdlib.h> 

#include "Utility/Utility.h"

using namespace ignis;

enum DemoProgram
{
	DEMO_TEXTURE,
	DEMO_INSTANCED,
	DEMO_FRAMEBUFFER,
	DEMO_MODEL,
	DEMO_MATERIAL,
	DEMO_NORMAL,
	DEMO_FONT,
	DEMO_COMPUTE_SHADER
};

DemoProgram PROG = DEMO_COMPUTE_SHADER;

// settings
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

glm::mat4 SCREEN_MAT = glm::ortho(0.0f, (float)WIDTH, (float)HEIGHT, 0.0f);

// mouse input
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;

float mouseOffsetX = 0.0f;
float mouseOffsetY = 0.0f;

bool firstMouse = true;

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

	vao.GenBuffer(GL_ARRAY_BUFFER);

	vao.SetBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices);
	vao.SetVertexAttribPointer(0, 2, 4, 0);
	vao.SetVertexAttribPointer(1, 2, 4, 2);

	vao.Unbind();

	Shader shader = Shader("res/shaders/texture.vert", "res/shaders/texture.frag");

	Texture texture = Texture("res/textures/texture.png");

	while (!glfwWindowShouldClose(window))
	{
		// input
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		glClear(GL_COLOR_BUFFER_BIT);

		glm::mat4 projection = glm::ortho(0.0f, 8.0f, 0.0f, 6.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 1.0f));

		vao.Bind();
		RenderTexture(texture, projection, view, model, shader, { 0, 1, 2, 2, 3, 0 });

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

	std::vector<glm::vec2> offsets;

	for (float y = 0.0f; y < 6.0f; y++)
	{
		for (float x = 0.0f; x < 8.0f; x++)
		{
			offsets.push_back(glm::vec2(x, y));
		}
	}

	vao.Bind();

	vao.GenBuffer(GL_ARRAY_BUFFER);

	vao.SetBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices);
	vao.SetVertexAttribPointer(0, 2, 4, 0);
	vao.SetVertexAttribPointer(1, 2, 4, 2);

	vao.GenBuffer(GL_ARRAY_BUFFER);
	vao.SetBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * offsets.size(), &offsets[0]);
	vao.SetVertexAttribPointer(2, 2, 2, 0);
	vao.SetVertexAttribDivisor(2, 1);

	vao.Unbind();

	Shader instanced = Shader("res/shaders/instanced.vert", "res/shaders/texture.frag");

	Texture texture = Texture("res/textures/texture.png");

	while (!glfwWindowShouldClose(window))
	{
		// input
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		glClear(GL_COLOR_BUFFER_BIT);

		glm::mat4 projection = glm::ortho(0.0f, 8.0f, 0.0f, 6.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 model = glm::mat4(1.0f);

		vao.Bind();

		RenderTextureInstanced(texture, 48, projection, view, model, instanced, { 0, 1, 2, 2, 3, 0 });

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

	vao.GenBuffer(GL_ARRAY_BUFFER);

	vao.SetBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices);
	vao.SetVertexAttribPointer(0, 2, 4, 0);
	vao.SetVertexAttribPointer(1, 2, 4, 2);

	vao.Unbind();

	Shader shader = Shader("res/shaders/texture.vert", "res/shaders/texture.frag");
	Shader inverted = Shader("res/shaders/texture.vert", "res/shaders/inverted.frag");
	Shader grayscale = Shader("res/shaders/texture.vert", "res/shaders/grayscale.frag");
	Shader kernel = Shader("res/shaders/texture.vert", "res/shaders/kernel.frag");

	Texture texture = Texture("res/textures/texture.png");

	FrameBuffer framebuffer = FrameBuffer(800, 600);

	while (!glfwWindowShouldClose(window))
	{
		// input
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		glClear(GL_COLOR_BUFFER_BIT);

		glm::mat4 projection = glm::ortho(0.0f, 8.0f, 0.0f, 6.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 1.0f));

		vao.Bind();

		framebuffer.Bind();
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		RenderTexture(texture, projection, view, model, shader, { 0, 1, 2, 2, 3, 0 });

		framebuffer.Unbind();

		framebuffer.BindVAO();
		RenderTexture(framebuffer.Texture(), glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f), kernel, { 0, 1, 2, 2, 3, 0 });

		vao.Unbind();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void DemoModel(GLFWwindow* window)
{
	FrameCounter frames;

	Font font = Font("res/fonts/OpenSans.ttf", 32.0f);
	Shader fontShader = Shader("res/shaders/font.vert", "res/shaders/font.frag");

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xPos, double yPos)
	{
		if (firstMouse)
		{
			lastX = (float)xPos;
			lastY = (float)yPos;
			firstMouse = false;
		}

		mouseOffsetX = (float)xPos - lastX;
		mouseOffsetY = lastY - (float)yPos; // reversed since y-coordinates go from bottom to top

		lastX = (float)xPos;
		lastY = (float)yPos;
	});

	Camera camera;
	float cameraSpeed = 2.5f;
	float cameraSensitivity = 0.1f;

	Shader shader = Shader("res/shaders/model.vert", "res/shaders/model.frag");

	Material material;
	Mesh mesh = Mesh::LoadFromFile("res/models/barrel2/barrel.obj", "res/models/barrel2/", &material);
	
	while (!glfwWindowShouldClose(window))
	{
		frames.Start((float)glfwGetTime());

		// input
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		float velocity = cameraSpeed * frames.DeltaTime;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera.Move(camera.Front * velocity);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.Move(-camera.Front * velocity);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera.Move(-camera.Right * velocity);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera.Move(camera.Right * velocity);
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			camera.Move(camera.WorldUp * velocity);
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			camera.Move(-camera.WorldUp * velocity);

		camera.YawPitch(mouseOffsetX * cameraSensitivity, mouseOffsetY * cameraSensitivity);
		mouseOffsetX = 0.0f;
		mouseOffsetY = 0.0f;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float fov = 70.0f;
		float aspect = (float)WIDTH / (float)HEIGHT;

		glm::mat4 projection = glm::perspective(fov, aspect, 0.1f, 100.0f);
		glm::mat4 view = camera.View();
		glm::mat4 model = glm::mat4(1.0f);

		RenderMesh(mesh, material, projection, view, model, shader);

		RenderText(fmt::format("FPS: {0}", frames.FPS), 0.0f, 32.0f, font, SCREEN_MAT, fontShader);

		glfwSwapBuffers(window);
		glfwPollEvents();

		frames.End((float)glfwGetTime());
	}
}

void DemoMaterial(GLFWwindow* window)
{
	FrameCounter frames;

	Camera camera;
	camera.Position = glm::vec3(0.0f, 0.0f, 3.0f);
	float cameraSpeed = 2.5f;
	float cameraSensitivity = 0.1f;

	Font font = Font("res/fonts/OpenSans.ttf", 32.0f);
	Shader fontShader = Shader("res/shaders/font.vert", "res/shaders/font.frag");

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xPos, double yPos)
	{
		if (firstMouse)
		{
			lastX = (float)xPos;
			lastY = (float)yPos;
			firstMouse = false;
		}

		mouseOffsetX = (float)xPos - lastX;
		mouseOffsetY = lastY - (float)yPos; // reversed since y-coordinates go from bottom to top

		lastX = (float)xPos;
		lastY = (float)yPos;
	});

	Timer timer;

	// load .obj file
	// current load time: 1717.34ms 
	Material material;
	Mesh mesh = Mesh::LoadFromFile("res/models/barrel2/barrel.obj", "res/models/barrel2/", &material);

	DEBUG_INFO("Loaded mesh in {0}ms", timer.GetElapsedMS());

	// load shader
	timer.Reset();
	Shader shader = Shader("res/shaders/material.vert", "res/shaders/material.frag");

	DEBUG_INFO("Loaded shader in {0}ms", timer.GetElapsedMS());

	Shader lampShader = Shader("res/shaders/lamp.vert", "res/shaders/lamp.frag");

	shader.Use();
	shader.SetUniform1i("diffuseMap", 0);
	shader.SetUniform1i("normalMap", 1);
	shader.SetUniform1i("specularMap", 2);

	// lamp
	float lampVertices[] =
	{
		// front
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		// back
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 // left
		 -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		 -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		 -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		 -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		 -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		 -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		 // right
		  0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		  0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		  0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		  0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		  0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		  // bot
		 -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		  0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		  0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		  0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 // top
		 -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		  0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		  0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f
	};

	VAO lampVao;
	lampVao.Bind();

	lampVao.GenBuffer(GL_ARRAY_BUFFER);
	lampVao.SetBufferData(GL_ARRAY_BUFFER, sizeof(lampVertices), lampVertices);
	lampVao.SetVertexAttribPointer(0, 3, 6 * sizeof(float), 0);
	lampVao.SetVertexAttribPointer(1, 3, 6 * sizeof(float), 3 * sizeof(float));
	
	// lighting
	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

	while (!glfwWindowShouldClose(window))
	{
		frames.Start((float)glfwGetTime());

		// input
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		float velocity = cameraSpeed * frames.DeltaTime;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera.Move(camera.Front * velocity);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.Move(-camera.Front * velocity);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera.Move(-camera.Right * velocity);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera.Move(camera.Right * velocity);
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			camera.Move(camera.WorldUp * velocity);
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			camera.Move(-camera.WorldUp * velocity);

		camera.YawPitch(mouseOffsetX * cameraSensitivity, mouseOffsetY * cameraSensitivity);
		mouseOffsetX = 0.0f;
		mouseOffsetY = 0.0f;

		lightPos = glm::rotate(lightPos, glm::radians(60.0f * frames.DeltaTime), glm::vec3(0.0f, 1.0f, 0.0f));

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float fov = 70.0f;
		float aspect = (float)WIDTH / (float)HEIGHT;

		glm::mat4 projection = glm::perspective(fov, aspect, 0.1f, 100.0f);
		glm::mat4 view = camera.View();
		glm::mat4 model = glm::mat4(1.0);
		
		shader.Use();
		shader.SetUniform3f("lightPos", lightPos);

		
		// render mesh 
		RenderMesh(mesh, material, projection, view, model, shader);
		
		// also draw the lamp object
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube

		glm::mat4 mvp = projection * view * model;

		lampShader.Use();
		lampShader.SetUniformMat4("mvp", mvp);

		lampVao.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);

		RenderText(fmt::format("FPS: {0}", frames.FPS), 0.0f, 32.0f, font, SCREEN_MAT, fontShader);

		glfwSwapBuffers(window);
		glfwPollEvents();

		frames.End((float)glfwGetTime());
	}
}

void DemoNormal(GLFWwindow* window)
{
	FrameCounter frames;

	Font font = Font("res/fonts/OpenSans.ttf", 32.0f);
	Shader fontShader = Shader("res/shaders/font.vert", "res/shaders/font.frag");

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xPos, double yPos)
	{
		if (firstMouse)
		{
			lastX = (float)xPos;
			lastY = (float)yPos;
			firstMouse = false;
		}

		mouseOffsetX = (float)xPos - lastX;
		mouseOffsetY = lastY - (float)yPos; // reversed since y-coordinates go from bottom to top

		lastX = (float)xPos;
		lastY = (float)yPos;
	});

	Camera camera;
	camera.Position = glm::vec3(0.0f, 0.0f, 3.0f);
	float cameraSpeed = 2.5f;
	float cameraSensitivity = 0.1f;

	// configure global opengl state
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// textures
	Texture diffuseMap = Texture("res/textures/brickwall.png");
	Texture normalMap = Texture("res/textures/brickwall_normal.png");

	// shader
	Shader lampShader = Shader("res/shaders/lamp.vert", "res/shaders/lamp.frag");

	Shader shader("res/shaders/normal.vert", "res/shaders/normal.frag");
	shader.Use();
	shader.SetUniform1i("diffuseMap", 0);
	shader.SetUniform1i("normalMap", 1);

	// quad
	// positions
	std::vector<glm::vec3> positions =
	{
		// front
		glm::vec3(-1.0f, -1.0f,  1.0f),
		glm::vec3( 1.0f, -1.0f,  1.0f),
		glm::vec3( 1.0f,  1.0f,  1.0f),
		glm::vec3( 1.0f,  1.0f,  1.0f),
		glm::vec3(-1.0f,  1.0f,  1.0f),
		glm::vec3(-1.0f, -1.0f,  1.0f),
		// back
		glm::vec3( 1.0f, -1.0f, -1.0f),
		glm::vec3(-1.0f, -1.0f, -1.0f),
		glm::vec3(-1.0f,  1.0f, -1.0f),
		glm::vec3(-1.0f,  1.0f, -1.0f),
		glm::vec3( 1.0f,  1.0f, -1.0f),
		glm::vec3( 1.0f, -1.0f, -1.0f),
		// left
		glm::vec3(-1.0f, -1.0f, -1.0f),
		glm::vec3(-1.0f, -1.0f,  1.0f),
		glm::vec3(-1.0f,  1.0f,  1.0f),
		glm::vec3(-1.0f,  1.0f,  1.0f),
		glm::vec3(-1.0f,  1.0f, -1.0f),
		glm::vec3(-1.0f, -1.0f, -1.0f),
		// right
		glm::vec3( 1.0f, -1.0f,  1.0f),
		glm::vec3( 1.0f, -1.0f, -1.0f),
		glm::vec3( 1.0f,  1.0f, -1.0f),
		glm::vec3( 1.0f,  1.0f, -1.0f),
		glm::vec3( 1.0f,  1.0f,  1.0f),
		glm::vec3( 1.0f, -1.0f,  1.0f),
		// top
		glm::vec3(-1.0f,  1.0f,  1.0f),
		glm::vec3( 1.0f,  1.0f,  1.0f),
		glm::vec3( 1.0f,  1.0f, -1.0f),
		glm::vec3( 1.0f,  1.0f, -1.0f),
		glm::vec3(-1.0f,  1.0f, -1.0f),
		glm::vec3(-1.0f,  1.0f,  1.0f),
		// bottom
		glm::vec3(-1.0f, -1.0f, -1.0f),
		glm::vec3( 1.0f, -1.0f, -1.0f),
		glm::vec3( 1.0f, -1.0f,  1.0f),
		glm::vec3( 1.0f, -1.0f,  1.0f),
		glm::vec3(-1.0f, -1.0f,  1.0f),
		glm::vec3(-1.0f, -1.0f, -1.0f)
	};

	// texture coordinates
	std::vector<glm::vec2> texcoords =
	{
		// front
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f),
		glm::vec2(0.0f, 0.0f),
		// back
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f),
		glm::vec2(0.0f, 0.0f),
		// left
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f),
		glm::vec2(0.0f, 0.0f),
		// right
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f),
		glm::vec2(0.0f, 0.0f),
		// top
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f),
		glm::vec2(0.0f, 0.0f),
		// bottom
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f),
		glm::vec2(0.0f, 0.0f),
	};

	// normal vector
	std::vector<glm::vec3> normals =
	{
		// front
		glm::vec3( 0.0f,  0.0f,  1.0f),
		glm::vec3( 0.0f,  0.0f,  1.0f),
		glm::vec3( 0.0f,  0.0f,  1.0f),
		glm::vec3( 0.0f,  0.0f,  1.0f),
		glm::vec3( 0.0f,  0.0f,  1.0f),
		glm::vec3( 0.0f,  0.0f,  1.0f),
		// back
		glm::vec3( 0.0f,  0.0f, -1.0f),
		glm::vec3( 0.0f,  0.0f, -1.0f),
		glm::vec3( 0.0f,  0.0f, -1.0f),
		glm::vec3( 0.0f,  0.0f, -1.0f),
		glm::vec3( 0.0f,  0.0f, -1.0f),
		glm::vec3( 0.0f,  0.0f, -1.0f),
		// left
		glm::vec3(-1.0f,  0.0f,  0.0f),
		glm::vec3(-1.0f,  0.0f,  0.0f),
		glm::vec3(-1.0f,  0.0f,  0.0f),
		glm::vec3(-1.0f,  0.0f,  0.0f),
		glm::vec3(-1.0f,  0.0f,  0.0f),
		glm::vec3(-1.0f,  0.0f,  0.0f),
		// right
		glm::vec3( 1.0f,  0.0f,  0.0f),
		glm::vec3( 1.0f,  0.0f,  0.0f),
		glm::vec3( 1.0f,  0.0f,  0.0f),
		glm::vec3( 1.0f,  0.0f,  0.0f),
		glm::vec3( 1.0f,  0.0f,  0.0f),
		glm::vec3( 1.0f,  0.0f,  0.0f),
		// top
		glm::vec3( 0.0f,  1.0f,  0.0f),
		glm::vec3( 0.0f,  1.0f,  0.0f),
		glm::vec3( 0.0f,  1.0f,  0.0f),
		glm::vec3( 0.0f,  1.0f,  0.0f),
		glm::vec3( 0.0f,  1.0f,  0.0f),
		glm::vec3( 0.0f,  1.0f,  0.0f),
		// bottom
		glm::vec3( 0.0f, -1.0f,  0.0f),
		glm::vec3( 0.0f, -1.0f,  0.0f),
		glm::vec3( 0.0f, -1.0f,  0.0f),
		glm::vec3( 0.0f, -1.0f,  0.0f),
		glm::vec3( 0.0f, -1.0f,  0.0f),
		glm::vec3( 0.0f, -1.0f,  0.0f)
	};

	std::vector<glm::vec3> tangents;
	std::vector<glm::vec3> bitangents;

	for (uint i = 0; i < positions.size(); i += 3)
	{
		glm::vec3 edge1 = positions[i + 1] - positions[i];
		glm::vec3 edge2 = positions[i + 2] - positions[i];
		glm::vec2 deltaUV1 = texcoords[i + 1] - texcoords[i];
		glm::vec2 deltaUV2 = texcoords[i + 2] - texcoords[i];

		GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		glm::vec3 tangent = (edge1 * deltaUV2.y - edge2 * deltaUV1.y) * f;
		glm::vec3 bitangent = (edge2 * deltaUV1.x - edge1 * deltaUV2.x) * f;
		tangent = glm::normalize(tangent);
		bitangent = glm::normalize(bitangent);

		tangents.push_back(tangent);
		tangents.push_back(tangent);
		tangents.push_back(tangent);

		bitangents.push_back(bitangent);
		bitangents.push_back(bitangent);
		bitangents.push_back(bitangent);
	}

	// configure plane VAO
	VAO quadVAO;
	quadVAO.Bind();

	quadVAO.GenBuffer(GL_ARRAY_BUFFER);
	quadVAO.SetBufferData(GL_ARRAY_BUFFER, sizeof(positions[0])* positions.size(), &positions[0]);
	quadVAO.SetVertexAttribPointer(0, 3, 0, 0);

	quadVAO.GenBuffer(GL_ARRAY_BUFFER);
	quadVAO.SetBufferData(GL_ARRAY_BUFFER, sizeof(texcoords[0])* texcoords.size(), &texcoords[0]);
	quadVAO.SetVertexAttribPointer(1, 2, 0, 0);

	quadVAO.GenBuffer(GL_ARRAY_BUFFER);
	quadVAO.SetBufferData(GL_ARRAY_BUFFER, sizeof(normals[0])* normals.size(), &normals[0]);
	quadVAO.SetVertexAttribPointer(2, 3, 0, 0);

	quadVAO.GenBuffer(GL_ARRAY_BUFFER);
	quadVAO.SetBufferData(GL_ARRAY_BUFFER, sizeof(tangents[0])* tangents.size(), &tangents[0]);
	quadVAO.SetVertexAttribPointer(3, 3, 0, 0);

	quadVAO.GenBuffer(GL_ARRAY_BUFFER);
	quadVAO.SetBufferData(GL_ARRAY_BUFFER, sizeof(bitangents[0])* bitangents.size(), &bitangents[0]);
	quadVAO.SetVertexAttribPointer(4, 3, 0, 0);

	// lighting info
	glm::vec3 lightPos(1.2f, 2.0f, 2.0f);

	// lamp
	float vertices[] = 
	{
		// front
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		// back
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		// left
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		// right
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 // bot
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		// top
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f
	};

	VAO lampVao;
	lampVao.Bind();

	lampVao.GenBuffer(GL_ARRAY_BUFFER);
	lampVao.SetBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices);
	lampVao.SetVertexAttribPointer(0, 3, 6, 0);
	lampVao.SetVertexAttribPointer(1, 3, 6, 3);

	while (!glfwWindowShouldClose(window))
	{
		frames.Start((float)glfwGetTime());

		// logic
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		float velocity = cameraSpeed * frames.DeltaTime;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera.Move(camera.Front * velocity);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.Move(-camera.Front * velocity);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera.Move(-camera.Right * velocity);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera.Move(camera.Right * velocity);
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			camera.Move(camera.WorldUp * velocity);
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			camera.Move(-camera.WorldUp * velocity);

		camera.YawPitch(mouseOffsetX * cameraSensitivity, mouseOffsetY * cameraSensitivity);
		mouseOffsetX = 0.0f;
		mouseOffsetY = 0.0f;

		lightPos = glm::rotate(lightPos, glm::radians(60.0f * frames.DeltaTime), glm::vec3(0.0f, 1.0f, 0.0f));

		// render
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// configure view/projection matrices
		float fov = 70.0f;
		float aspect = (float)WIDTH / (float)HEIGHT;

		glm::mat4 projection = glm::perspective(fov, aspect, 0.1f, 100.0f);
		glm::mat4 view = camera.View();
		glm::mat4 model = glm::mat4(1.0f);

		shader.Use();
		shader.SetUniformMat4("projection", projection);
		shader.SetUniformMat4("view", view);
		shader.SetUniformMat4("model", model);

		// render normal-mapped quad
		shader.SetUniform3f("viewPos", camera.Position);
		shader.SetUniform3f("lightPos", lightPos);

		diffuseMap.Bind(0);
		normalMap.Bind(1);

		quadVAO.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// render lamp
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube

		lampShader.Use();
		lampShader.SetUniformMat4("projection", projection);
		lampShader.SetUniformMat4("view", view);
		lampShader.SetUniformMat4("model", model);

		lampVao.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);

		RenderText(fmt::format("FPS: {0}", frames.FPS), 0.0f, 32.0f, font, SCREEN_MAT, fontShader);

		glfwSwapBuffers(window);
		glfwPollEvents();

		frames.End((float)glfwGetTime());
	}
}

void DemoFont(GLFWwindow* window)
{
	FrameCounter frames;
	glEnable(GL_DEPTH_TEST);

	Font font = Font("res/fonts/OpenSans.ttf", 32.0f);
	Shader fontShader = Shader("res/shaders/font.vert", "res/shaders/font.frag");

	while (!glfwWindowShouldClose(window))
	{
		frames.Start((float)glfwGetTime());

		// input
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// ----------------------------------------------------------------
		
		RenderText(fmt::format("FPS: {0}", frames.FPS), 0.0f, 32.0f, font, SCREEN_MAT, fontShader);

		// ----------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();

		frames.End((float)glfwGetTime());
	}
}

glm::vec2 mousePos = { WIDTH / 2.0f, HEIGHT / 2.0f };
float mouseScroll = 0.0f;

const int PARTICLE_GROUP_SIZE = 64;
const int PARTICLE_GROUP_COUNT = 128;
const int PARTICLE_COUNT = (PARTICLE_GROUP_SIZE * PARTICLE_GROUP_COUNT);

void DemoComputeShader(GLFWwindow* window)
{
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

	Shader shader = Shader("res/shaders/test.vert", "res/shaders/test.frag");

	GLuint compShader = CreateComputeShader(ReadFile("res/shaders/trail.comp").c_str());
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
		RenderText(fmt::format("Particle density: {0}", particleSize), 0.0f, 96.0f, font, SCREEN_MAT, fontShader);

		glfwSwapBuffers(window);
		glfwPollEvents();

		frames.End((float)glfwGetTime());
	}

	glDeleteBuffers(1, &bufferPosition);
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

	switch (PROG)
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
	case DEMO_MATERIAL:
		DemoMaterial(window);
		break;
	case DEMO_NORMAL:
		DemoNormal(window);
		break;
	case DEMO_FONT:
		DemoFont(window);
		break;
	case DEMO_COMPUTE_SHADER:
		DemoComputeShader(window);
		break;
	}
	
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
#endif
