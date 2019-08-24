#include "Buffer.h"

VAO::VAO()
{
	glGenVertexArrays(1, &id);
	glBindVertexArray(id);
}

VAO::~VAO()
{
	glDeleteVertexArrays(1, &id);
	glDeleteBuffers(buffers.size(), buffers.data());
}

void VAO::Bind() const
{
	glBindVertexArray(id);
}

void VAO::Unbind() const
{
	glBindVertexArray(0);
}

unsigned int VAO::GenVertexBuffer()
{
	unsigned int vbo;

	glGenBuffers(1, &vbo);

	buffers.push_back(vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	return buffers.size() - 1;
}

void VAO::BindVertexBuffer(uint index)
{
	if (index < buffers.size())
		glBindBuffer(GL_ARRAY_BUFFER, buffers.at(index));
}

void VAO::UnbindVertexBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VAO::SetVertexBufferData(size_t size, const void* data, int usage)
{
	glBufferData(GL_ARRAY_BUFFER, size, data, (GLenum)usage);
}

void VAO::SetVertexAttribPointer(uint index, int size, size_t stride, int offset)
{
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset * sizeof(float)));
}

void VAO::SetVertexAttribIPointer(uint index, int size, size_t stride, int offset)
{
	glEnableVertexAttribArray(index);
	glVertexAttribIPointer(index, size, GL_UNSIGNED_INT, stride * sizeof(GLuint), (void*)(offset * sizeof(GLuint)));
}

void VAO::SetVertexAttribDivisor(uint index, unsigned int divisor)
{
	glVertexAttribDivisor(index, divisor);
}

void VAO::MapBufferData(uint index, const void* data, size_t size)
{
	BindVertexBuffer(index);

	void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

	memcpy(ptr, data, size);

	glUnmapBuffer(GL_ARRAY_BUFFER);
}
