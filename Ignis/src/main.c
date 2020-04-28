#include "Application/Application.h"

#include "Camera/Camera.h"

#include "Ignis/Ignis.h"

Camera camera;

GLuint VAO;
GLuint Buffer;

IgnisShader shader;

void OnInit(Application* app)
{
	/* ---------------| Config |------------------------------------------ */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	FontRendererInit("res/shaders/font.vert", "res/shaders/font.frag");
	FontRendererBindFont(ResourceManagerGetFont(&app->resources, "gui"), IGNIS_WHITE);

	ApplicationEnableDebugMode(app, 1);
	ApplicationEnableVsync(app, 0);
	ApplicationShowGui(app, 0);

	/* ------------------------------------------------------------------- */
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &Buffer);
	glBindBuffer(GL_ARRAY_BUFFER, Buffer);

	GLfloat vertices[6][2] =
	{
		{ -0.90f, -0.90f }, {  0.85f, -0.90f }, { -0.90f,  0.85f },  // Triangle 1
		{  0.90f, -0.85f }, {  0.90f,  0.90f }, { -0.85f,  0.90f }   // Triangle 2
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	ignisCreateShadervf(&shader, "res/shaders/triangles.vert", "res/shaders/triangles.frag");

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, IGNIS_BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
}

void OnDestroy(Application* app)
{
	FontRendererDestroy();

	ignisDeleteShader(&shader);
}

void OnEvent(Application* app, const Event e)
{
	if (e.type == EVENT_WINDOW_RESIZE)
	{
		ApplicationSetViewport(app, 0, 0, e.window.width, e.window.height);
	}

	if (e.type == EVENT_KEY_PRESSED)
	{
		switch (e.key.keycode)
		{
		case KEY_ESCAPE:
			ApplicationClose(app);
			break;
		}
	}
}

void OnUpdate(Application* app, float deltaTime)
{

}

void OnRender(Application* app)
{
	ignisUseShader(&shader);

	static const float black[] = { 0.0f, 0.0f, 0.0f, 0.0f };

	glClearBufferfv(GL_COLOR, 0, black);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void OnRenderDebug(Application* app)
{
	/* fps */
	FontRendererStart(ApplicationGetScreenProjPtr(app));

	FontRendererRenderTextFormat(8.0f, 20.0f, "FPS: %d", app->timer.fps);

	FontRendererFlush();
}

void OnRenderGui(Application* app)
{
	
}

int main()
{
	Application app;
	ApplicationLoadConfig(&app, "config.json");

	OnInit(&app);

	ApplicationSetOnEventCallback(&app, OnEvent);
	ApplicationSetOnUpdateCallback(&app, OnUpdate);
	ApplicationSetOnRenderCallback(&app, OnRender);
	ApplicationSetOnRenderDebugCallback(&app, OnRenderDebug);
	ApplicationSetOnRenderGuiCallback(&app, OnRenderGui);

	ApplicationRun(&app);

	ApplicationDestroy(&app);

	OnDestroy(&app);

	return 0;
}