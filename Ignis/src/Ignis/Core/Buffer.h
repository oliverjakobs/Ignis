#pragma once

#include <glad/glad.h>

#include "Types.h"

namespace ignis
{
	struct Buffer
	{
		uint Target;
		uint Name;

		Buffer(uint target);
		virtual ~Buffer();

		void Bind() const;
		void Unbind() const;
	};

	struct ArrayBuffer : public Buffer
	{
		ArrayBuffer();

		void BufferData(uint size, const void* data, uint usage = GL_STATIC_DRAW);
		void BufferSubData(uint offset, uint size, const void* data);

		void VertexAttribPointer(uint index, uint size, uint stride, uint offset);
		void VertexAttribIPointer(uint index, uint size, uint stride, uint offset);
		void VertexAttribDivisor(uint index, uint divisor);

		void* MapBuffer(uint access);
		void* MapBufferRange(uint offset, uint length, uint access);

		template <typename Type> Type* MapBuffer(uint access);
		template <typename Type> Type* MapBufferRange(uint offset, uint count, uint access);

		void UnmapBuffer();
	};

	template <typename Type>
	inline Type* ArrayBuffer::MapBuffer(uint access)
	{
		return (Type*)MapBuffer(access);
	}

	template <typename Type>
	inline Type* ArrayBuffer::MapBufferRange(uint offset, uint count, uint access)
	{
		return (Type*)MapBufferRange(offset, count * sizeof(Type), access);
	}

	struct ElementBuffer : public Buffer
	{
		uint Count;

		ElementBuffer();

		void BufferData(uint count, const uint* data, uint usage = GL_STATIC_DRAW);
	};

	// TODO: Handling buffers through here
	struct VertexArray
	{
		uint Name;

		VertexArray();
		~VertexArray();

		void Bind() const;
		void Unbind() const;
	};

	// Advanced buffers
	struct TextureBuffer
	{
		uint Texture;
		uint Format;

		TextureBuffer(uint format, uint buffer);
		~TextureBuffer();

		void BindImageTexture(uint unit, uint access);
	};

	struct RenderBuffer
	{
		uint Name;

		RenderBuffer();
		~RenderBuffer();

		void Bind() const;
		void Unbind() const;

		void RenderbufferStorage(uint format, int width, int height);
	};
}