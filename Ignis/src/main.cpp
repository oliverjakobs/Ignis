#include "Ignis/Ignis.h"

#include <GLFW/glfw3.h>

#include "Ignis/Advanced/ComputeShader.h"

#include "Obelisk/Obelisk.h"
#include "ImGuiBinding/ImGuiRenderer.h"

#include "Tile/TileRenderer.h"

#include "Primitives/Primitives.h"

using namespace ignis;
using namespace tile;

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
	
	Renderer2D::Init();
	Primitives::Init();

	// need to be loaded from file
	std::vector<TileID> tileIDs =
	{
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
		0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
		0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
		0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,
		0,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,0,
		0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
		0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
		0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
		1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
		0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
		0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
		0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
		0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
		0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
		0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
		0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
		0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
		0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,
		0,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,0,
		0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
		0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
		0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
		0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1
	};

	TypeMap typeMap =
	{
		{ 1, TileType::TILE_SOLID },
		{ 2, TileType::TILE_SLOPE_LEFT },
		{ 3, TileType::TILE_SLOPE_RIGHT },
		{ 4, TileType::TILE_PLATFORM }
	};

	int mapWidth = 32;
	int mapHeight = 25;
	float tileSize = 32.0f;
	size_t chunkSize = 16;

	int texRows = 1;
	int texColumns = 5;

	// load map
	TileMap map = TileMap(tileIDs, mapWidth, mapHeight, tileSize, chunkSize, typeMap);
	TileRenderer tileRenderer(map, std::make_shared<Texture>("res/textures/tiles.png"), texRows, texColumns);

	auto edges = GetEdges(map[0].Tiles, map.GetChunkSize(), map.GetChunkSize(), map.GetTileSize());
	auto tiles = GetTiles(map);

	while (!glfwWindowShouldClose(window))
	{
		timer.Start((float)glfwGetTime());

		// input
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		glClear(GL_COLOR_BUFFER_BIT);

		tileRenderer.RenderMap(glm::vec3(), camera.GetViewProjection());

		Primitives::Start(camera.GetViewProjection());

		//for (auto& tile : tiles)
		//{
		//	if (tile.Type == TileType::TILE_SOLID)
		//		Primitives::DrawRect(tile.Position, glm::vec2(map.GetTileSize()));
		//}

		for (auto& edge : edges)
		{
			Primitives::DrawLine(edge.Start, edge.End);
		}

		//auto& activeChunk = map.at(map.GetIndex(mousePos));
		//Primitives::DrawRect(activeChunk.Position * static_cast<float>(chunkSize) * tileSize, glm::vec2(chunkSize * tileSize));

		//// TODO: Move into compute shader
		//auto vertices = Visibility(mousePos, edges);

		//// Draw each triangle in fan
		//if (!vertices.empty())
		//{
		//	for (size_t i = 0; i < vertices.size() - 1; i++)
		//	{
		//		Primitives::DrawPolygon({ mousePos, {vertices[i].x, vertices[i].y}, {vertices[i + 1].x, vertices[i + 1].y} });

		//	}
		//	Primitives::DrawPolygon({ mousePos, {vertices.back().x, vertices.back().y}, {vertices.front().x, vertices.front().y} });
		//}

		Primitives::Flush();

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