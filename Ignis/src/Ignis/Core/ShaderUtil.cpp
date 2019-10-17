#include "ShaderUtil.h"

#include "Obelisk/Debugger.h"

namespace ignis
{
	uint CreateShaderProgram(std::map<uint, const std::string&> shaderSrc)
	{
		uint program = glCreateProgram();
		std::vector<uint> attachedShader;

		for (auto& [type, src] : shaderSrc)
		{
			uint shader = CompileShader(type, src);

			if (shader == 0)
			{
				glDeleteProgram(program);
				return 0;
			}

			glAttachShader(program, shader);
			attachedShader.push_back(shader);
		}

		glLinkProgram(program);

		for (auto& shader : attachedShader)
		{
			glDeleteShader(shader);
			glDetachShader(program, shader);
		}

		GLint result = GL_FALSE;
		glGetProgramiv(program, GL_LINK_STATUS, &result);
		if (result == GL_FALSE)
		{
			DEBUG_ERROR("[SHADER] Linking Error");
			DEBUG_ERROR("[SHADER] {0}", GetProgramLog(program));
			glDeleteProgram(program);
			return 0;
		}

		glValidateProgram(program);

		result = GL_FALSE;
		glGetProgramiv(program, GL_VALIDATE_STATUS, &result);
		if (result == GL_FALSE)
		{
			DEBUG_ERROR("[SHADER] Validating Error");
			DEBUG_ERROR("[SHADER] {0}", GetProgramLog(program));
			glDeleteProgram(program);

			return 0;
		}

		return program;
	}

	uint CompileShader(uint type, const std::string& source)
	{
		if (source.empty())
		{
			DEBUG_ERROR("[SHADER] Shader source is missing for {0}", GetShaderType(type));
			return 0;
		}

		uint shader = glCreateShader(type);

		const char* data = source.c_str();
		glShaderSource(shader, 1, &data, nullptr);
		glCompileShader(shader);

		GLint result = GL_FALSE;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE)
		{
			DEBUG_ERROR("[SHADER] Compiling Error ({0})", GetShaderType(type));
			DEBUG_ERROR("[SHADER] {0}", GetShaderLog(shader));
			glDeleteShader(shader);

			return 0;
		}

		return shader;
	}

	std::string GetShaderLog(uint object)
	{
		GLint logLength = 0;

		if (!glIsShader(object))
			return "Failed to log: Object is not a shader";

		glGetShaderiv(object, GL_INFO_LOG_LENGTH, &logLength);

		std::vector<GLchar> log(logLength);

		glGetShaderInfoLog(object, logLength, &logLength, &log[0]);

		return std::string(&log[0], logLength - 1);
	}

	std::string GetProgramLog(uint object)
	{
		GLint logLength = 0;

		if (!glIsProgram(object))
			return "Failed to log: Object is not a program";

		glGetProgramiv(object, GL_INFO_LOG_LENGTH, &logLength);

		std::vector<GLchar> log(logLength);

		glGetProgramInfoLog(object, logLength, &logLength, &log[0]);

		return std::string(&log[0], logLength - 1);
	}

	std::string GetShaderType(uint type)
	{
		switch (type)
		{
		case GL_COMPUTE_SHADER:
			return "GL_COMPUTE_SHADER";
		case GL_VERTEX_SHADER:
			return "GL_VERTEX_SHADER";
		case GL_TESS_CONTROL_SHADER:
			return "GL_TESS_CONTROL_SHADER";
		case GL_TESS_EVALUATION_SHADER:
			return "GL_TESS_EVALUATION_SHADER";
		case GL_GEOMETRY_SHADER:
			return "GL_GEOMETRY_SHADER";
		case GL_FRAGMENT_SHADER:
			return "GL_FRAGMENT_SHADER";
		}

		return "INVALID_SHADER_TYPE";
	}
}