#pragma once

#include <glad/glad.h>

#include "Types.h"

namespace ignis
{
	struct Buffer
	{
		uint ID;
		uint Type;
	};

	struct VAO
	{
		uint ID;
		std::vector<Buffer> Buffers;

		VAO();
		~VAO();

		void Bind() const;
		void Unbind() const;

		uint GetBufferType(uint index) const;

		void BindBuffer(uint index);
		void UnbindBuffer(uint type);
		void UnbindVertexBuffer();
		void UnbindIndexBuffer();

		// vertex buffers
		uint GenVertexBuffer();

		void SetVertexBufferData(uint size, const void* data, int usage = GL_STATIC_DRAW);
		void SetVertexAttribPointer(uint index, int size, uint stride, int offset);
		void SetVertexAttribIPointer(uint index, int size, uint stride, int offset);
		void SetVertexAttribDivisor(uint index, uint divisor);

		// index buffer
		uint GenIndexBuffer();
		void SetIndexBufferData(uint size, const void* data, int usage = GL_STATIC_DRAW);

		void MapBufferData(uint index, const void* data, size_t size);
	};
}