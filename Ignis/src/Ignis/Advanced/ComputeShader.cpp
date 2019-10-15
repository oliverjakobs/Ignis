#include "ComputeShader.h"

#include "Obelisk/Obelisk.h"

namespace ignis
{
	ComputeShader::ComputeShader(const std::string& path)
	{
		m_program = glCreateProgram();

		uint shader = CompileShader(GL_COMPUTE_SHADER, obelisk::ReadFile(path));

		glAttachShader(m_program, shader);
		glLinkProgram(m_program);

		GLint result = GL_FALSE;
		glGetProgramiv(m_program, GL_LINK_STATUS, &result);
		if (result == GL_FALSE)
		{
			DEBUG_ERROR("[SHADER] Linking Error");
			DEBUG_ERROR("[SHADER] {0}", GetProgramLog(m_program));
			glDeleteShader(shader);
			glDeleteProgram(m_program);

			return;
		}

		glValidateProgram(m_program);

		result = GL_FALSE;
		glGetProgramiv(m_program, GL_VALIDATE_STATUS, &result);
		if (result == GL_FALSE)
		{
			DEBUG_ERROR("[SHADER] Validating Error");
			DEBUG_ERROR("[SHADER] {0}", GetProgramLog(m_program));
			glDeleteShader(shader);
			glDeleteProgram(m_program);

			return;
		}

		glDeleteShader(shader);
	}

	void ComputeShader::Dispatch(uint num_groups_x, uint num_groups_y, uint num_groups_z, uint barrier_bits)
	{
		glDispatchCompute(num_groups_x, num_groups_y, num_groups_z);

		// prevent sampling befor all writes to image are done
		glMemoryBarrier(barrier_bits);
	}
}