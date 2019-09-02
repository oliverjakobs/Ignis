#include "Ignis/Ignis.h"
#include "Ignis/Framebuffer.h"

#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

#include <glm/gtx/transform.hpp>


#include "Ignis/Mesh.h"

#define STB_TRUETYPE_IMPLEMENTATION  // force following include to generate implementation
#include "stb_truetype.h"

unsigned char ttf_buffer[1 << 20];
unsigned char temp_bitmap[512 * 512];

stbtt_bakedchar cdata[96]; // ASCII 32..126 is 95 glyphs
GLuint ftex;

GLuint fVAO, fVBO;

void my_stbtt_initfont(void)
{
	fread(ttf_buffer, 1, 1 << 20, fopen("c:/windows/fonts/times.ttf", "rb"));
	stbtt_BakeFontBitmap(ttf_buffer, 0, 32.0, temp_bitmap, 512, 512, 32, 96, cdata); // no guarantee this fits!
	// can free ttf_buffer at this point
	glGenTextures(1, &ftex);
	glBindTexture(GL_TEXTURE_2D, ftex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 512, 512, 0, GL_RED, GL_UNSIGNED_BYTE, temp_bitmap);
	// can free temp_bitmap at this point
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glGenVertexArrays(1, &fVAO);
	glGenBuffers(1, &fVBO);
	glBindVertexArray(fVAO);
	glBindBuffer(GL_ARRAY_BUFFER, fVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void my_stbtt_print(float x, float y, char* text, Shader* shader)
{
	// assume orthographic projection with units = screen pixels, origin at top left
	glBindVertexArray(fVAO);
	glActiveTexture(GL_TEXTURE0);

	while (*text) 
	{
		if (*text >= 32 && *text < 128) 
		{
			stbtt_aligned_quad q;
			stbtt_GetBakedQuad(cdata, 512, 512, *text - 32, &x, &y, &q, 1);//1=opengl & d3d10+,0=d3d9

			GLfloat vertices[6][4] = {
			{ q.x0, q.y1, q.s0, q.t0 },
			{ q.x0, q.y0, q.s0, q.t1 },
			{ q.x1, q.y0, q.s1, q.t1 },

			{ q.x0, q.y1, q.s0, q.t0 },
			{ q.x1, q.y0, q.s1, q.t1 },
			{ q.x1, q.y1, q.s1, q.t0 }
			};

			glBindTexture(GL_TEXTURE_2D, ftex);
			// Update content of VBO memory
			glBindBuffer(GL_ARRAY_BUFFER, fVBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			// Render quad
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		++text;
	}
}

enum DemoProgram
{
	DEMO_TEXTURE,
	DEMO_INSTANCED,
	DEMO_ALPHA,
	DEMO_FRAMEBUFFER,
	DEMO_FONT,
	DEMO_MODEL
};

int main()
{
	Ignis ignis;

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
	GLFWwindow* window = glfwCreateWindow(800, 600, "Ignis", nullptr, nullptr);
	if (window == nullptr)
	{
		DEBUG_ERROR("[GLFW] Failed to create GLFW window");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	DEBUG_INFO("[GLFW] Window created.");

	// Set GLFW callbacks
	glfwSetErrorCallback([](int error, const char* desc)
	{
		DEBUG_ERROR("[GLFW] ({0}) {1}", error, desc);
	});

	bool debug = true;

	if (!ignis.LoadGL(glfwGetProcAddress, debug))
	{
		DEBUG_ERROR("[IGNIS] Failed to init renderer");
		glfwTerminate();
		return -1;
	}

	ignis.EnableBlend(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	ignis.SetClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	VAO vao;

	GLfloat vertices[] =
	{
		0.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f
	};

	vertices2 offsets;

	for (float y = 0.0f; y < 6.0f; y++)
	{
		for (float x = 0.0f; x < 8.0f; x++)
		{
			offsets.push_back(glm::vec2(x, y));
		}
	}

	vao.Bind();

	vao.GenVertexBuffer();

	vao.SetVertexBufferData(sizeof(vertices), vertices);
	vao.SetVertexAttribPointer(0, 2, 4, 0);
	vao.SetVertexAttribPointer(1, 2, 4, 2);

	vao.GenVertexBuffer();
	vao.SetVertexBufferData(sizeof(glm::vec2) * offsets.size(), &offsets[0]);
	vao.SetVertexAttribPointer(2, 2, 2, 0);
	vao.SetVertexAttribDivisor(2, 1);

	vao.UnbindVertexBuffer();

	Shader* shader = new Shader("res/shaders/texture.vert", "res/shaders/texture.frag");
	Shader* instanced = new Shader("res/shaders/instanced.vert", "res/shaders/texture.frag");
	Shader* inverted = new Shader("res/shaders/texture.vert", "res/shaders/inverted.frag");
	Shader* grayscale = new Shader("res/shaders/texture.vert", "res/shaders/grayscale.frag");
	Shader* kernel = new Shader("res/shaders/texture.vert", "res/shaders/kernel.frag");

	Shader* font = new Shader("res/shaders/font.vert", "res/shaders/font.frag");

	Shader* test = new Shader("res/shaders/test.vert", "res/shaders/test.frag");

	Texture* texture = new Texture("res/textures/texture.png");

	Renderer renderer;

	DemoProgram prog = DEMO_MODEL;

	my_stbtt_initfont();

	Mesh box = Mesh("res/models/barrel.obj");
	Texture* boxTex = new Texture("res/models/barrel.png");
	Shader* modelShader = new Shader("res/shaders/model.vert", "res/shaders/model.frag");

	// --------------------------------------------------------------------
	FrameBuffer framebuffer = FrameBuffer(800, 600);
	
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glm::mat4 projection = glm::mat4(1.0f);
		glm::mat4 view = glm::ortho(0.0f, 8.0f, 0.0f, 6.0f);
		glm::mat4 model = glm::mat4(1.0f);

		vao.Bind();

		switch (prog)
		{
		case DEMO_TEXTURE:
			model = glm::translate(model, glm::vec3(2.0f, 1.0f, 1.0f));
			model = glm::scale(model, glm::vec3(4.0f, 4.0f, 1.0f));

			renderer.RenderTexture(texture, projection, view, model, test, { 0, 1, 2, 2, 3, 0 });
			break;
		case DEMO_INSTANCED:
			renderer.RenderTextureInstanced(texture, 48, projection, view, model, instanced, { 0, 1, 2, 2, 3, 0 });
			break;
		case DEMO_ALPHA:
			break;
		case DEMO_FRAMEBUFFER:
			framebuffer.Bind();
			//glEnable(GL_DEPTH_TEST);

			// make sure we clear the framebuffer's content
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			model = glm::translate(model, glm::vec3(2.0f, 1.0f, 1.0f));
			model = glm::scale(model, glm::vec3(4.0f, 4.0f, 1.0f));

			renderer.RenderTexture(texture, projection, view, model, shader, { 0, 1, 2, 2, 3, 0 });

			framebuffer.Unbind();
			//glDisable(GL_DEPTH_TEST); 

			framebuffer.VAO().Bind();
			renderer.RenderTexture(framebuffer.Texture(), glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f), kernel, { 0, 1, 2, 2, 3, 0 });
			break;
		case DEMO_FONT:
			font->Use();

			font->SetUniformMat4("projection", projection);

			my_stbtt_print(0.0f, 0.0f, "Hello Font", font);
			break;
		case DEMO_MODEL:
			modelShader->Use();

			glm::vec3 pos = glm::vec3(0.0f, 0.0f, -5.0f);
			glm::vec3 forward = glm::vec3(0.0f, 0.0f, 1.0f);
			glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

			glm::mat4 persp = glm::perspective(70.0f, 800.0f/600.0f, 0.1f, 100.0f);

			modelShader->SetUniformMat4("MVP", persp* glm::lookAt(pos, pos + forward, up));
			modelShader->SetUniformMat4("Normal", glm::mat4(1.0f));
			modelShader->SetUniform3f("lightDirection", glm::vec3(0.0f, 0.0f, 1.0f));
			boxTex->Bind();
			box.Draw();
			break;
		default:
			break;
		}

		vao.Unbind();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	SAFE_DELETE(shader);
	SAFE_DELETE(texture);
	
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}