#include "Ignis/Ignis.h"

#include <GLFW/glfw3.h>

#include <glm/gtc/random.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp> 

#include "Ignis/Advanced/ComputeShader.h"

#include "Obelisk/Obelisk.h"
#include "ImGuiBinding/ImGuiRenderer.h"

#include "Ignis/Camera/FpsCamera.h"

#include "Tile/TileRenderer.h"

#include "Maths/Visibility.h"
#include "Primitives/Primitives.h"

using namespace ignis;

// #define ENABLE_MOVEMENT

// settings
int WIDTH = 1024;
int HEIGHT = 800;

glm::vec2 SCREEN_CENTER = { WIDTH / 2.0f, HEIGHT / 2.0f };

// mouse input
glm::vec2 mousePos = SCREEN_CENTER;

float cameraSpeed = 50.0f;
float cameraRotation = 180.0f;

OrthographicCamera camera = OrthographicCamera(0.0f, (float)WIDTH, 0.0f, (float)HEIGHT);

GLFWwindow* Init(const char* title, int width, int height);

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
	//renderState.SetDepthTest(true);
	//renderState.SetCullFace(true);
	
	Renderer2D::Init();

	std::shared_ptr<Texture> texture = std::make_shared<Texture>("res/textures/checkerboard.png");

	// load map
	std::vector<int> tiles =
	{
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
		0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
		0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
		0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
		0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,
		0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
		0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
		0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
		0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
		0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
		0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
		0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
		0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
		0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
		0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
		0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
		0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
		0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
		0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,
		0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
		0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
		0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
		0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1
	};

	std::map<unsigned int, TileType> typeMap = 
	{
		{ 1, TileType::TILE_SOLID },
		{ 2, TileType::TILE_SLOPE_LEFT },
		{ 3, TileType::TILE_SLOPE_RIGHT },
		{ 4, TileType::TILE_PLATFORM }
	};

	std::shared_ptr<Texture> tileTexture = std::make_shared<Texture>("res/textures/tiles.png");

	int mapWidth = 32;
	int mapHeight = 25;
	float tileSize = 32.0f;

	int texRows = 1;
	int texColumns = 5;

	TileMap map = TileMap(tiles, mapWidth, mapHeight, tileSize, typeMap);
	TileRenderer tileRenderer(map.GetTiles().size());

	tileRenderer.LoadMap(map.GetTiles(), texRows, texColumns, map.GetTileSize());

	auto edges = map.ToEdges();

	Primitives::Init();

	while (!glfwWindowShouldClose(window))
	{
		timer.Start((float)glfwGetTime());

		// input
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

#ifdef ENABLE_MOVEMENT
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

#endif 

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		tileRenderer.RenderMap(glm::vec3(), camera.GetViewProjection(), tileTexture);

		// TODO: Move into compute shader
		auto vertices = GetVisibilityPolygonPoints(mousePos, edges, 100.0f);

		Primitives::Start(camera.GetViewProjection());

		// Draw each triangle in fan
		if (!vertices.empty())
		{
			for (size_t i = 0; i < vertices.size() - 1; i++)
			{
				Primitives::DrawPolygon({ mousePos, vertices[i].pos, vertices[i + 1].pos });

			}
			Primitives::DrawPolygon({ mousePos, vertices.back().pos, vertices.front().pos });
		}

		Primitives::Flush();

		//Renderer2D::BeginScene(camera.GetViewProjection());

		//Renderer2D::RenderQuad({ -1.0f, 0.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		//Renderer2D::RenderQuad({ 0.5f, -0.5f, 0.0f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		//Renderer2D::RenderQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, texture);
		//Renderer2D::RenderQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, tileTexture);

		//Renderer2D::EndScene();

		// gui
		// ImGuiRenderer::Begin();
		// ImGuiRenderer::End();

		// debug info
		fontShader.Use();
		fontShader.SetUniformMat4("u_Projection", ignisScreenMat());
		fontShader.SetUniform4f("u_Color", WHITE);

		FontRenderer::RenderText(font, obelisk::format("FPS: %d", timer.FPS), 0.0f, 32.0f);

		glfwSwapBuffers(window);
		glfwPollEvents();

		timer.End((float)glfwGetTime());
	}
	
	Primitives::Destroy();
	ImGuiRenderer::Quit();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

GLFWwindow* Init(const char* title, int width, int height)
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

	glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height)
	{
		WIDTH = width;
		HEIGHT = height;

		glm::vec2 SCREEN_CENTER = { width / 2.0f, height / 2.0f };

		camera.SetProjection(0.0f, (float)width, 0.0f, (float)height);

		ignisViewport(0, 0, width, height);
	});

	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xPos, double yPos)
	{
		mousePos = { (float)xPos, HEIGHT - (float)yPos };
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

	return window;
}