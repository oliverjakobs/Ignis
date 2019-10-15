#pragma once

#include "Ignis/Core/Shader.h"

namespace ignis
{
	class ComputeShader : public Shader
	{
	public:
		ComputeShader(const std::string& path);

		void Dispatch(uint num_groups_x, uint num_groups_y, uint num_groups_z, uint barrier_bits = GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	};
}