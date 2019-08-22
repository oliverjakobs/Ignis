#include "Renderer.h"

#include "Utility.h"

#include <glad/glad.h>

void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	DEBUG_MESSAGE("OpenGL debug output ({0}): {1}", id, message);

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:				DEBUG_MESSAGE("Source: API"); break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:		DEBUG_MESSAGE("Source: Window System"); break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:	DEBUG_MESSAGE("Source: Shader Compiler"); break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:		DEBUG_MESSAGE("Source: Third Party"); break;
	case GL_DEBUG_SOURCE_APPLICATION:		DEBUG_MESSAGE("Source: Application"); break;
	case GL_DEBUG_SOURCE_OTHER:				DEBUG_MESSAGE("Source: Other"); break;
	}

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:				DEBUG_MESSAGE("Type: Error"); break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:	DEBUG_MESSAGE("Type: Deprecated Behaviour"); break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:	DEBUG_MESSAGE("Type: Undefined Behaviour"); break;
	case GL_DEBUG_TYPE_PORTABILITY:			DEBUG_MESSAGE("Type: Portability"); break;
	case GL_DEBUG_TYPE_PERFORMANCE:			DEBUG_MESSAGE("Type: Performance"); break;
	case GL_DEBUG_TYPE_MARKER:				DEBUG_MESSAGE("Type: Marker"); break;
	case GL_DEBUG_TYPE_PUSH_GROUP:			DEBUG_MESSAGE("Type: Push Group"); break;
	case GL_DEBUG_TYPE_POP_GROUP:			DEBUG_MESSAGE("Type: Pop Group"); break;
	case GL_DEBUG_TYPE_OTHER:				DEBUG_MESSAGE("Type: Other"); break;
	}

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:			DEBUG_MESSAGE("Severity: high"); break;
	case GL_DEBUG_SEVERITY_MEDIUM:			DEBUG_MESSAGE("Severity: medium"); break;
	case GL_DEBUG_SEVERITY_LOW:				DEBUG_MESSAGE("Severity: low"); break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:	DEBUG_MESSAGE("Severity: notification"); break;
	}
}

Renderer::~Renderer()
{
}

bool Renderer::Init(void* loadProc, bool debug)
{
	// loading glad
	if (!gladLoadGLLoader((GLADloadproc)loadProc))
	{
		DEBUG_MESSAGE("[GLAD] Failed to initialize GLAD");
		return false;
	}
	
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

			DEBUG_MESSAGE("[OpenGL] Debug context created");
		}
		else
		{
			DEBUG_MESSAGE("[OpenGL] Could not create debug context");
		}
	}

	DEBUG_MESSAGE("[GLAD] Initialized");
	DEBUG_MESSAGE("[OpenGL] Version: ",		<< glGetString(GL_VERSION));
	DEBUG_MESSAGE("[OpenGL] Vendor: ",		<< glGetString(GL_VENDOR));
	DEBUG_MESSAGE("[OpenGL] Renderer: "		<< glGetString(GL_RENDERER));
	DEBUG_MESSAGE("[OpenGL] GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION));

	return true;
}
