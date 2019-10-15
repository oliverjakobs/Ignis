#pragma once

#include <glad/glad.h>

#include "Types.h"

namespace ignis
{
	uint CreateShader(const std::string& vertSrc, const std::string& geomSrc, const std::string& fragSrc);
	uint CompileShader(uint type, const std::string& source);

	std::string GetShaderLog(uint object);
	std::string GetProgramLog(uint object);
	std::string GetShaderType(uint type);
}