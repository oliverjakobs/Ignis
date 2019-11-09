#pragma once

#include "Ignis/Core/Shader.h"

namespace ignis
{
	class ComputeShader : public Shader
	{
	public:
		ComputeShader(const std::string& path);

		void Dispatch(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z, GLbitfield barriers);
	};
}