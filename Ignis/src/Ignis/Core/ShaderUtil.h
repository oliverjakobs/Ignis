#pragma once

#include <glad/glad.h>

#include "Types.h"

namespace ignis
{
	uint CreateShaderProgram(std::map<uint, const std::string&> shadersrc);
	uint CompileShader(uint type, const std::string& source);

	std::string GetShaderLog(uint object);
	std::string GetProgramLog(uint object);
	std::string GetShaderType(uint type);
}