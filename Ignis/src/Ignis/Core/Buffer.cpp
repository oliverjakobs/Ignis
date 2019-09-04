#include "Buffer.h"

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

void VAO::BindBuffer(uint index)
{
	if (index < Buffers.size())
	{
		Buffer buffer = Buffers.at(index);
		glBindBuffer(buffer.Type, buffer.ID);
	}
}

void VAO::UnbindVertexBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VAO::UnbindIndexBuffer()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

uint VAO::GenVertexBuffer()
{
	uint vbo;

	glGenBuffers(1, &vbo);

	Buffers.push_back({ vbo, GL_ARRAY_BUFFER });

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	return Buffers.size() - 1;
}

void VAO::SetVertexBufferData(uint size, const void* data, int usage)
{
	glBufferData(GL_ARRAY_BUFFER, size, data, (GLenum)usage);
}

void VAO::SetVertexAttribPointer(uint index, int size, uint stride, int offset)
{
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset * sizeof(float)));
}

void VAO::SetVertexAttribIPointer(uint index, int size, uint stride, int offset)
{
	glEnableVertexAttribArray(index);
	glVertexAttribIPointer(index, size, GL_UNSIGNED_INT, stride * sizeof(GLuint), (void*)(offset * sizeof(GLuint)));
}

void VAO::SetVertexAttribDivisor(uint index, uint divisor)
{
	glVertexAttribDivisor(index, divisor);
}

uint VAO::GenIndexBuffer()
{
	uint ibo;

	glGenBuffers(1, &ibo);

	Buffers.push_back({ ibo, GL_ELEMENT_ARRAY_BUFFER });

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	return Buffers.size() - 1;
}

void VAO::SetIndexBufferData(uint size, const void* data, int usage)
{
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, (GLenum)usage);
}

void VAO::MapBufferData(uint index, const void* data, size_t size)
{
	BindBuffer(index);

	void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

	memcpy(ptr, data, size);

	glUnmapBuffer(GL_ARRAY_BUFFER);
}
