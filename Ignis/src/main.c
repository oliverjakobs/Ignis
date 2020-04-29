#include "Application/Application.h"

#include "Camera/Camera.h"

#include "Ignis/Ignis.h"

Camera camera;

GLuint VAO;
GLuint Buffer;

IgnisVertexArray vao;
IgnisShader shader;

void OnInit(Application* app)
{
	/* ---------------| Config |------------------------------------------ */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	FontRendererInit("res/shaders/font.vert", "res/shaders/font.frag");
	FontRendererBindFont(ResourceManagerGetFont(&app->resources, "gui"), IGNIS_WHITE);

	ApplicationEnableDebugMode(app, 1);
	ApplicationEnableVsync(app, 0);
	ApplicationShowGui(app, 0);

	/* ------------------------------------------------------------------- */
	ignisGenerateVertexArray(&vao);

	GLfloat vertices[6][2] =
	{
		/* Triangle 1 */
		{ -0.90f, -0.90f },
		{  0.85f, -0.90f },
		{ -0.90f,  0.85f },
		/* Triangle 2 */
		{  0.90f, -0.85f },
		{  0.90f,  0.90f },
		{ -0.85f,  0.90f }
	};

	ignisAddArrayBuffer(&vao, sizeof(vertices), vertices, GL_STATIC_DRAW);
	ignisVertexAttribPointer(0, 2, GL_FALSE, 0, 0);

	ignisCreateShadervf(&shader, "res/shaders/triangles.vert", "res/shaders/triangles.frag");
}

void OnDestroy(Application* app)
{
	FontRendererDestroy();

	ignisDeleteShader(&shader);
}

void OnEvent(Application* app, Event e)
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

	ignisClearColorBuffer((IgnisColorRGBA){ 0.2f, 0.2f, 0.2f, 1.0f });

	ignisBindVertexArray(&vao);
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

	ApplicationSetOnInitCallback(&app, OnInit);
	ApplicationSetOnDestroyCallback(&app, OnDestroy);
	ApplicationSetOnEventCallback(&app, OnEvent);
	ApplicationSetOnUpdateCallback(&app, OnUpdate);
	ApplicationSetOnRenderCallback(&app, OnRender);
	ApplicationSetOnRenderDebugCallback(&app, OnRenderDebug);
	ApplicationSetOnRenderGuiCallback(&app, OnRenderGui);

	ApplicationLoadConfig(&app, "config.json");

	ApplicationRun(&app);

	ApplicationDestroy(&app);

	return 0;
}