#include "Shader.h"

#include <glad/glad.h>

#include "Utility.h"

unsigned int Shader::CreateShader(const std::string& vertSrc, const std::string& fragSrc)
{
	unsigned int program = glCreateProgram();

	unsigned int vert = CompileShader(GL_VERTEX_SHADER, vertSrc.c_str());
	//unsigned int geom = compileShader(GL_GEOMETRY_SHADER, geomSrc);
	unsigned int frag = CompileShader(GL_FRAGMENT_SHADER, fragSrc.c_str());

	if (vert == 0 || frag == 0)
	{
		DEBUG_MESSAGE("Shader Missing");
		return 0;
	}

	glAttachShader(program, vert);
	glAttachShader(program, frag);

	glLinkProgram(program);

	GLint result = GL_FALSE;
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	if (result == GL_FALSE)
	{
		PrintLog(program);
		glDeleteProgram(program);

		DEBUG_MESSAGE("Linking Error");
		return 0;
	}

	glValidateProgram(program);

	result = GL_FALSE;
	glGetProgramiv(program, GL_VALIDATE_STATUS, &result);
	if (result == GL_FALSE)
	{
		PrintLog(program);
		glDeleteProgram(program);

		DEBUG_MESSAGE("Validating Error");
		return 0;
	}

	glDeleteShader(vert);
	glDeleteShader(frag);

	return program;
}

unsigned int Shader::CompileShader(uint type, const char* source)
{
	if (*source == 0)
		return 0;

	unsigned int shader = glCreateShader(type);

	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);

	GLint result = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		PrintLog(shader);
		glDeleteShader(shader);

		DEBUG_MESSAGE("Compiling Error: " << std::to_string(type));

		return 0;
	}

	return shader;
}

void Shader::PrintLog(uint object)
{
	GLint log_length = 0;
	if (glIsShader(object))
		glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_length);
	else if (glIsProgram(object))
		glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_length);
	else
	{
		DEBUG_MESSAGE("Shader Log: Not a shader or a program");
		return;
	}

	char* log = (char*)malloc(log_length);

	if (glIsShader(object))
		glGetShaderInfoLog(object, log_length, NULL, log);
	else if (glIsProgram(object))
		glGetProgramInfoLog(object, log_length, NULL, log);

	DEBUG_MESSAGE(log);
	free(log);
}

Shader::Shader(const std::string& vs, const std::string& fs)
{
	m_program = CreateShader(ReadFile(vs.c_str()).c_str(), ReadFile(fs.c_str()).c_str());
}

Shader::~Shader()
{
	glDeleteProgram(m_program);
}

void Shader::use()
{
	glUseProgram(m_program);
}

void Shader::setUniform1i(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(m_program, name.c_str()), value);
}

void Shader::setUniform1f(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(m_program, name.c_str()), value);
}

void Shader::setUniform2f(const std::string& name, const glm::vec2& vector) const
{
	glUniform2fv(glGetUniformLocation(m_program, name.c_str()), 1, &vector[0]);
}

void Shader::setUniform3f(const std::string& name, const glm::vec3& vector) const
{
	glUniform3fv(glGetUniformLocation(m_program, name.c_str()), 1, &vector[0]);
}

void Shader::setUniform4f(const std::string& name, const glm::vec4& vector) const
{
	glUniform4fv(glGetUniformLocation(m_program, name.c_str()), 1, &vector[0]);
}

void Shader::setUniformMat2(const std::string& name, const glm::mat4& matrix) const
{
	glUniformMatrix2fv(glGetUniformLocation(m_program, name.c_str()), 1, GL_FALSE, &matrix[0][0]);
}

void Shader::setUniformMat3(const std::string& name, const glm::mat4& matrix) const
{
	glUniformMatrix3fv(glGetUniformLocation(m_program, name.c_str()), 1, GL_FALSE, &matrix[0][0]);
}

void Shader::setUniformMat4(const std::string& name, const glm::mat4& matrix) const
{
	glUniformMatrix4fv(glGetUniformLocation(m_program, name.c_str()), 1, GL_FALSE, &matrix[0][0]);
}