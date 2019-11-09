#include "ComputeShader.h"

#include "Obelisk/Obelisk.h"

namespace ignis
{
	ComputeShader::ComputeShader(const std::string& path)
	{
		m_program = CreateShaderProgram({ { GL_COMPUTE_SHADER, obelisk::ReadFile(path) } });
	}

	void ComputeShader::Dispatch(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z, GLbitfield barriers)
	{
		glDispatchCompute(num_groups_x, num_groups_y, num_groups_z);

		// prevent sampling befor all writes to image are done
		glMemoryBarrier(barriers);
	}
}