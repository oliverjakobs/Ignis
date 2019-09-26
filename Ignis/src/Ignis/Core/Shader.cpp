#include "Shader.h"

#include "Ignis/Utility/Utility.h"

namespace ignis
{
	// Helper functions for logging
	std::string GetShaderLog(uint object);
	std::string GetProgramLog(uint object);
	std::string GetShaderType(uint type);

	uint Shader::CreateShader(const std::string& vertSrc, const std::string& geomSrc, const std::string& fragSrc)
	{
		uint program = glCreateProgram();

		uint vert = CompileShader(GL_VERTEX_SHADER, vertSrc);
		uint frag = CompileShader(GL_FRAGMENT_SHADER, fragSrc);

		if (vert == 0 || frag == 0)
			return 0;

		glAttachShader(program, vert);
		glAttachShader(program, frag);

		uint geom = 0;

		// optional shader
		if (!geomSrc.empty())
			geom = CompileShader(GL_GEOMETRY_SHADER, geomSrc);

		if (geom != 0)
			glAttachShader(program, geom);

		glLinkProgram(program);

		GLint result = GL_FALSE;
		glGetProgramiv(program, GL_LINK_STATUS, &result);
		if (result == GL_FALSE)
		{
			DEBUG_ERROR("[IGNIS] Linking Error");
			DEBUG_ERROR("[IGNIS] {0}", GetProgramLog(program));
			glDeleteProgram(program);

			return 0;
		}

		glValidateProgram(program);

		result = GL_FALSE;
		glGetProgramiv(program, GL_VALIDATE_STATUS, &result);
		if (result == GL_FALSE)
		{
			DEBUG_ERROR("[IGNIS] Validating Error");
			DEBUG_ERROR("[IGNIS] {0}", GetProgramLog(program));
			glDeleteProgram(program);

			return 0;
		}

		glDeleteShader(vert);
		glDeleteShader(frag);

		glDeleteShader(geom);

		return program;
	}

	uint Shader::CompileShader(uint type, const std::string& source)
	{
		if (source.empty())
		{
			DEBUG_ERROR("[IGNIS] Shader source is missing for {0}", GetShaderType(type));
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
			DEBUG_ERROR("[IGNIS] Compiling Error ({0})", GetShaderType(type));
			DEBUG_ERROR("[IGNIS] {0}", GetShaderLog(shader));
			glDeleteShader(shader);

			return 0;
		}

		return shader;
	}

	Shader::Shader(const std::string& vert, const std::string& frag)
	{
		m_program = CreateShader(ReadFile(vert), "", ReadFile(frag));

		if (!glIsProgram(m_program))
			DEBUG_ERROR("[IGNIS] Failed to create shader from {0} or {1}", vert, frag);
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_program);
	}

	void Shader::Use()
	{
		DEBUG_ASSERT(glIsProgram(m_program), "Program handle does not refer to an object generated by OpenGL.");
		glUseProgram(m_program);
	}

	void Shader::SetUniform1i(const std::string& name, int value) const
	{
		int location = GetUniformLocation(name);

		if (location < 0)
			DEBUG_WARN("[Ignis] Uniform {0} not found", name);
		else
			glUniform1i(location, value);
	}

	void Shader::SetUniform1f(const std::string& name, float value) const
	{
		int location = GetUniformLocation(name);

		if (location < 0)
			DEBUG_WARN("[Ignis] Uniform {0} not found", name);
		else
			glUniform1f(location, value);
	}

	void Shader::SetUniform2f(const std::string& name, const glm::vec2& vector) const
	{
		int location = GetUniformLocation(name);

		if (location < 0)
			DEBUG_WARN("[Ignis] Uniform {0} not found", name);
		else
			glUniform2fv(location, 1, &vector[0]);
	}

	void Shader::SetUniform3f(const std::string& name, const glm::vec3& vector) const
	{
		int location = GetUniformLocation(name);

		if (location < 0)
			DEBUG_WARN("[Ignis] Uniform {0} not found", name);
		else
			glUniform3fv(location, 1, &vector[0]);
	}

	void Shader::SetUniform4f(const std::string& name, const glm::vec4& vector) const
	{
		int location = GetUniformLocation(name);

		if (location < 0)
			DEBUG_WARN("[Ignis] Uniform {0} not found", name);
		else
			glUniform4fv(location, 1, &vector[0]);
	}

	void Shader::SetUniformMat2(const std::string& name, const glm::mat4& matrix) const
	{
		int location = GetUniformLocation(name);

		if (location < 0)
			DEBUG_WARN("[Ignis] Uniform {0} not found", name);
		else
			glUniformMatrix2fv(location, 1, GL_FALSE, &matrix[0][0]);
	}

	void Shader::SetUniformMat3(const std::string& name, const glm::mat4& matrix) const
	{
		int location = GetUniformLocation(name);

		if (location < 0)
			DEBUG_WARN("[Ignis] Uniform {0} not found", name);
		else
			glUniformMatrix3fv(location, 1, GL_FALSE, &matrix[0][0]);
	}

	void Shader::SetUniformMat4(const std::string& name, const glm::mat4& matrix) const
	{
		int location = GetUniformLocation(name);

		if (location < 0)
			DEBUG_WARN("[Ignis] Uniform {0} not found", name);
		else
			glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
	}

	int Shader::GetUniformLocation(const std::string& name) const
	{
		return glGetUniformLocation(m_program, name.c_str());
	}

	void Shader::SetUniform1i(int location, int value) const
	{
		glUniform1i(location, value);
	}

	void Shader::SetUniform1f(int location, float value) const
	{
		glUniform1f(location, value);
	}

	void Shader::SetUniform2f(int location, const glm::vec2& vector) const
	{
		glUniform2fv(location, 1, &vector[0]);
	}

	void Shader::SetUniform3f(int location, const glm::vec3& vector) const
	{
		glUniform3fv(location, 1, &vector[0]);
	}

	void Shader::SetUniform4f(int location, const glm::vec4& vector) const
	{
		glUniform4fv(location, 1, &vector[0]);
	}

	void Shader::SetUniformMat2(int location, const glm::mat4& matrix) const
	{
		glUniformMatrix2fv(location, 1, GL_FALSE, &matrix[0][0]);
	}

	void Shader::SetUniformMat3(int location, const glm::mat4& matrix) const
	{
		glUniformMatrix3fv(location, 1, GL_FALSE, &matrix[0][0]);
	}

	void Shader::SetUniformMat4(int location, const glm::mat4& matrix) const
	{
		glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
	}

	// ---------------------------------------------------------------------------------------------------
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