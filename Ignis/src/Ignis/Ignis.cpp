#include "Ignis.h"

#include "Core/Utility.h"

#include <glad/glad.h>

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

Ignis::Ignis()
{
	Logger::SetFormat("[%^%l%$] %v");
	Logger::SetLevel(LogLevel::Trace);
}

Ignis::~Ignis()
{
}

bool Ignis::LoadGL(void* loadProc, bool debug)
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

void Ignis::EnableBlend(uint sfactor, uint dfactor)
{
	glEnable(GL_BLEND);
	glBlendFunc(static_cast<GLenum>(sfactor), static_cast<GLenum>(dfactor));
}

void Ignis::SetClearColor(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
}