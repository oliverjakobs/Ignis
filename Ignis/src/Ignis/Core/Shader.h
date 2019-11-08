#pragma once

#include <glad/glad.h>

#include "Api.h"

namespace ignis
{
	class Shader
	{
	protected:
		uint m_program;

		Shader() : m_program(0) {};

	public:
		Shader(const std::string& vert, const std::string& frag);
		Shader(const std::string& vert, const std::string& geom, const std::string& frag);
		virtual ~Shader();

		void Use();

		void SetUniform1i(const std::string& name, int value) const;
		void SetUniform1f(const std::string& name, float value) const;
		void SetUniform2f(const std::string& name, const glm::vec2& vector) const;
		void SetUniform3f(const std::string& name, const glm::vec3& vector) const;
		void SetUniform4f(const std::string& name, const glm::vec4& vector) const;
		void SetUniformMat2(const std::string& name, const glm::mat4& matrix) const;
		void SetUniformMat3(const std::string& name, const glm::mat4& matrix) const;
		void SetUniformMat4(const std::string& name, const glm::mat4& matrix) const;

		int GetUniformLocation(const std::string& name) const;

		void SetUniform1i(int location, int value) const;
		void SetUniform1f(int location, float value) const;
		void SetUniform2f(int location, const glm::vec2& vector) const;
		void SetUniform3f(int location, const glm::vec3& vector) const;
		void SetUniform4f(int location, const glm::vec4& vector) const;
		void SetUniformMat2(int location, const glm::mat4& matrix) const;
		void SetUniformMat3(int location, const glm::mat4& matrix) const;
		void SetUniformMat4(int location, const glm::mat4& matrix) const;
	};

	// Shader utils
	uint CreateShaderProgram(std::map<uint, const std::string&> sources);
	uint CompileShader(uint type, const std::string& source);

	std::string GetShaderLog(uint object);
	std::string GetProgramLog(uint object);
	std::string GetShaderType(uint type);
}