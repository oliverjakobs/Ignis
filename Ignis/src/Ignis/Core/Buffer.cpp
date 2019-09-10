#include "Buffer.h"

namespace ignis
{
	VAO::VAO()
	{
		glGenVertexArrays(1, &ID);
		glBindVertexArray(ID);
	}

	VAO::~VAO()
	{
		glDeleteVertexArrays(1, &ID);

		for (auto& b : Buffers)
			glDeleteBuffers(1, &b.ID);
	}

	void VAO::Bind() const
	{
		glBindVertexArray(ID);
	}

	void VAO::Unbind() const
	{
		glBindVertexArray(0);
	}

	uint VAO::GetBufferType(uint index) const
	{
		if (index < Buffers.size())
			return Buffers.at(index).Type;

		return 0;
	}

	void VAO::BindBuffer(uint index)
	{
		if (index < Buffers.size())
		{
			Buffer& buffer = Buffers.at(index);
			glBindBuffer(buffer.Type, buffer.ID);
		}
	}

	void VAO::UnbindBuffer(uint type)
	{
		glBindBuffer(type, 0);
	}

	uint VAO::GenBuffer(uint type)
	{
		uint buffer;

		glGenBuffers(1, &buffer);
		glBindBuffer(type, buffer);

		Buffers.push_back({ buffer, type });

		return Buffers.size() - 1;
	}

	void VAO::SetBufferData(uint type, uint size, const void* data, uint usage)
	{
		glBufferData(type, size, data, (GLenum)usage);
	}

	void VAO::SetBufferSubData(uint target, uint offset, uint size, const void* data)
	{
		glBufferSubData(target, offset, size, data);
	}

	void VAO::SetVertexAttribPointer(uint index, uint size, uint stride, uint offset)
	{
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset * sizeof(float)));
	}

	void VAO::SetVertexAttribIPointer(uint index, uint size, uint stride, uint offset)
	{
		glEnableVertexAttribArray(index);
		glVertexAttribIPointer(index, size, GL_UNSIGNED_INT, stride * sizeof(GLuint), (void*)(offset * sizeof(GLuint)));
	}

	void VAO::SetVertexAttribDivisor(uint index, uint divisor)
	{
		glVertexAttribDivisor(index, divisor);
	}

	void VAO::MapBufferData(uint index, const void* data, uint size)
	{
		if (index < Buffers.size())
		{
			Buffer& buffer = Buffers.at(index);
			glBindBuffer(buffer.Type, buffer.ID);

			void* ptr = glMapBuffer(buffer.Type, GL_WRITE_ONLY);

			memcpy(ptr, data, size);

			glUnmapBuffer(buffer.Type);
		}
	}
}