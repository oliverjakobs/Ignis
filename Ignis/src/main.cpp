#include "Ignis/Ignis.h"

#include "Ignis/Framebuffer.h"
#include "Ignis/Camera.h"
#include "Ignis/Timer.h"

#include <GLFW/glfw3.h>

#include <spdlog/fmt/fmt.h>

using namespace ignis;

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
	Timer timer;

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
		timer.Start((float)glfwGetTime());

		// input
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		float velocity = cameraSpeed * timer.DeltaTime;
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

		RenderText(fmt::format("FPS: {0}", timer.FPS), 0.0f, 32.0f, font, SCREEN_MAT, fontShader);

		glfwSwapBuffers(window);
		glfwPollEvents();

		timer.End((float)glfwGetTime());
	}
}

void DemoMaterial(GLFWwindow* window)
{
	Timer timer;

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

	Shader shader = Shader("res/shaders/material.vert", "res/shaders/material.frag");
	Shader lampShader = Shader("res/shaders/lamp.vert", "res/shaders/lamp.frag");

	Material material;
	Mesh mesh = Mesh::LoadFromFile("res/models/barrel2/barrel.obj", "res/models/barrel2/", &material);

	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	VAO lampVao;
	lampVao.Bind();

	lampVao.GenBuffer(GL_ARRAY_BUFFER);
	lampVao.SetBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices);
	lampVao.SetVertexAttribPointer(0, 3, 6, 0);
	lampVao.SetVertexAttribPointer(1, 3, 6, 3);

	// load shader
	shader.Use();
	shader.SetUniform1i("diffuseMap", 0);
	shader.SetUniform1i("normalMap", 1);

	// lighting
	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
	shader.SetUniform3f("lightPos", lightPos);

	while (!glfwWindowShouldClose(window))
	{
		timer.Start((float)glfwGetTime());

		// input
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		float velocity = cameraSpeed * timer.DeltaTime;
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

		shader.Use();
		shader.SetUniform3f("viewPos", camera.Position);

		float fov = 70.0f;
		float aspect = (float)WIDTH / (float)HEIGHT;

		glm::mat4 projection = glm::perspective(fov, aspect, 0.1f, 100.0f);
		glm::mat4 view = camera.View();
		glm::mat4 model = glm::mat4(1.0f);

		// render mesh
		glm::mat4 mvp = projection * view * model;

		shader.Use();

		shader.SetUniformMat4("mvp", mvp);
		shader.SetUniformMat4("model", model);

		if (material.Diffuse)
			material.Diffuse->Bind(0);

		if (material.Normal)
			material.Normal->Bind(1);

		mesh.VAO().Bind();
		glDrawElementsBaseVertex(GL_TRIANGLES, mesh.NumIndices(), GL_UNSIGNED_INT, 0, 0);

		// also draw the lamp object
		lampShader.Use();
		lampShader.SetUniformMat4("projection", projection);
		lampShader.SetUniformMat4("view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
		lampShader.SetUniformMat4("model", model);

		lampVao.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);

		RenderText(fmt::format("FPS: {0}", timer.FPS), 0.0f, 32.0f, font, SCREEN_MAT, fontShader);

		glfwSwapBuffers(window);
		glfwPollEvents();

		timer.End((float)glfwGetTime());
	}
}

void DemoFont(GLFWwindow* window)
{
	Timer timer;
	glEnable(GL_DEPTH_TEST);

	Font font = Font("res/fonts/OpenSans.ttf", 32.0f);
	Shader fontShader = Shader("res/shaders/font.vert", "res/shaders/font.frag");

	while (!glfwWindowShouldClose(window))
	{
		timer.Start((float)glfwGetTime());

		// input
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// ----------------------------------------------------------------
		
		RenderText(fmt::format("FPS: {0}", timer.FPS), 0.0f, 32.0f, font, SCREEN_MAT, fontShader);

		// ----------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();

		timer.End((float)glfwGetTime());
	}
}

enum DemoProgram
{
	DEMO_TEXTURE,
	DEMO_INSTANCED,
	DEMO_FRAMEBUFFER,
	DEMO_MODEL,
	DEMO_MATERIAL,
	DEMO_FONT,
	DEMO_ALPHA
};

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

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	DemoProgram prog = DEMO_MATERIAL;

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
	case DEMO_MATERIAL:
		DemoMaterial(window);
		break;
	case DEMO_FONT:
		DemoFont(window);
		break;
	}
	
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
