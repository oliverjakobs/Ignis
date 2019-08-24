#pragma once

#include <glad/glad.h>

#include "Types.h"

struct VAO
{
	unsigned int id;

	std::vector<uint> buffers;

	VAO();
	~VAO();

	void Bind() const;
	void Unbind() const;

	// vertex buffers
	unsigned int GenVertexBuffer();
	void BindVertexBuffer(uint index);
	void UnbindVertexBuffer();

	void SetVertexBufferData(size_t size, const void* data, int usage = GL_STATIC_DRAW);
	void SetVertexAttribPointer(uint index, int size, size_t stride, int offset);
	void SetVertexAttribIPointer(uint index, int size, size_t stride, int offset);
	void SetVertexAttribDivisor(uint index, uint divisor);

	void MapBufferData(uint index, const void* data, size_t size);
};