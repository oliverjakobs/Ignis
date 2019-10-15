#include "Ignis.h"

namespace ignis
{
	void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
	{
		// ignore non-significant error/warning codes
		if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

		DEBUG_ERROR("OpenGL debug output ({0}): {1}", id, message);

		switch (source)
		{
		case GL_DEBUG_SOURCE_API:				DEBUG_ERROR("Source: API"); break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:		DEBUG_ERROR("Source: Window System"); break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER:	DEBUG_ERROR("Source: Shader Compiler"); break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:		DEBUG_ERROR("Source: Third Party"); break;
		case GL_DEBUG_SOURCE_APPLICATION:		DEBUG_ERROR("Source: Application"); break;
		case GL_DEBUG_SOURCE_OTHER:				DEBUG_ERROR("Source: Other"); break;
		}

		switch (type)
		{
		case GL_DEBUG_TYPE_ERROR:				DEBUG_ERROR("Type: Error"); break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:	DEBUG_ERROR("Type: Deprecated Behaviour"); break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:	DEBUG_ERROR("Type: Undefined Behaviour"); break;
		case GL_DEBUG_TYPE_PORTABILITY:			DEBUG_ERROR("Type: Portability"); break;
		case GL_DEBUG_TYPE_PERFORMANCE:			DEBUG_ERROR("Type: Performance"); break;
		case GL_DEBUG_TYPE_MARKER:				DEBUG_ERROR("Type: Marker"); break;
		case GL_DEBUG_TYPE_PUSH_GROUP:			DEBUG_ERROR("Type: Push Group"); break;
		case GL_DEBUG_TYPE_POP_GROUP:			DEBUG_ERROR("Type: Pop Group"); break;
		case GL_DEBUG_TYPE_OTHER:				DEBUG_ERROR("Type: Other"); break;
		}

		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:			DEBUG_ERROR("Severity: high"); break;
		case GL_DEBUG_SEVERITY_MEDIUM:			DEBUG_ERROR("Severity: medium"); break;
		case GL_DEBUG_SEVERITY_LOW:				DEBUG_ERROR("Severity: low"); break;
		case GL_DEBUG_SEVERITY_NOTIFICATION:	DEBUG_ERROR("Severity: notification"); break;
		}
	}

	bool InitIgnis()
	{
		obelisk::Logger::SetFormat("[%^%l%$] %v");
		obelisk::Logger::SetLevel(obelisk::LogLevel::Trace);

		return true;
	}

	bool LoadGL(void* loadProc, bool debug)
	{
		// loading glad
		if (!gladLoadGLLoader((GLADloadproc)loadProc))
		{
			DEBUG_ERROR("[GLAD] Failed to initialize GLAD");
			return false;
		}

		DEBUG_INFO("[GLAD] Initialized");

		if (debug)
		{
			//Set up opengl debug output
			GLint flags, minor, major;
			glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
			glGetIntegerv(GL_MINOR_VERSION, &minor);
			glGetIntegerv(GL_MAJOR_VERSION, &major);

			if ((flags & GL_CONTEXT_FLAG_DEBUG_BIT) && (major >= 4 && minor >= 3))
			{
				glEnable(GL_DEBUG_OUTPUT);
				glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
				glDebugMessageCallback(glDebugOutput, nullptr);
				glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

				DEBUG_INFO("[OpenGL] Debug context created");
			}
			else
			{
				DEBUG_ERROR("[OpenGL] Could not create debug context");
			}
		}

		DEBUG_INFO("[OpenGL] Version: {0}", glGetString(GL_VERSION));
		DEBUG_INFO("[OpenGL] Vendor: {0}", glGetString(GL_VENDOR));
		DEBUG_INFO("[OpenGL] Renderer: {0}", glGetString(GL_RENDERER));
		DEBUG_INFO("[OpenGL] GLSL Version: {0}", glGetString(GL_SHADING_LANGUAGE_VERSION));

		return true;
	}

	void RenderTexture(Texture& tex, glm::mat4 proj, glm::mat4 view, glm::mat4 model, Shader& shader, int first, uint count)
	{
		glm::mat4 mvp = proj * view * model;

		shader.Use();

		shader.SetUniformMat4("mvp", mvp);

		tex.Bind();
		glDrawArrays(GL_TRIANGLES, first, count);
	}

	void RenderTexture(Texture& tex, glm::mat4 proj, glm::mat4 view, glm::mat4 model, Shader& shader, std::vector<uint> indices)
	{
		glm::mat4 mvp = proj * view * model;

		shader.Use();

		shader.SetUniformMat4("mvp", mvp);

		tex.Bind();
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data());
		tex.Unbind();
	}

	void RenderTextureInstanced(Texture& tex, uint instances, glm::mat4 proj, glm::mat4 view, glm::mat4 model, Shader& shader, std::vector<uint> indices)
	{
		glm::mat4 mvp = proj * view * model;

		shader.Use();

		shader.SetUniformMat4("mvp", mvp);

		tex.Bind();
		glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data(), instances);
		tex.Unbind();
	}

	void RenderMesh(Mesh& mesh, Texture& tex, glm::mat4 proj, glm::mat4 view, glm::mat4 model, Shader& shader)
	{
		glm::mat4 mvp = proj * view * model;

		shader.Use();

		shader.SetUniformMat4("mvp", mvp);
		shader.SetUniformMat4("model", model);

		tex.Bind();

		mesh.VAO().Bind();
		glDrawElementsBaseVertex(GL_TRIANGLES, mesh.NumIndices(), GL_UNSIGNED_INT, 0, 0);
		mesh.VAO().Unbind();

		tex.Unbind();
	}

	void RenderMesh(Mesh& mesh, Material& mtl, glm::mat4 proj, glm::mat4 view, glm::mat4 model, Shader& shader)
	{
		glm::mat4 mvp = proj * view * model;

		shader.Use();

		shader.SetUniformMat4("mvp", mvp);
		shader.SetUniformMat4("model", model);

		mtl.Bind();

		mesh.VAO().Bind();
		glDrawElementsBaseVertex(GL_TRIANGLES, mesh.NumIndices(), GL_UNSIGNED_INT, 0, 0);
		mesh.VAO().Unbind();

		mtl.Unbind();
	}
	
	void RenderText(const std::string& text, float x, float y, Font& font, glm::mat4 proj, Shader& shader)
	{
		shader.Use();
		shader.SetUniformMat4("projection", proj);

		font.Bind();

		for (auto& c : text)
		{
			if (font.LoadCharQuad(c, &x, &y))
			{
				// Render quad
				std::vector<uint> indices = { 0,1,2,2,3,0 };
				glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data());
			}
		}
	}
}