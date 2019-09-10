#pragma once

#include <glad/glad.h>

#include "Types.h"

namespace ignis
{
	class Shader
	{
	private:
		uint m_program;
	public:
		Shader(const std::string& vert, const std::string& frag);
		~Shader();

		static uint CreateShader(const std::string& vertSrc, const std::string& geomSrc, const std::string& fragSrc);
		static uint CompileShader(uint type, const std::string& source);

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
}