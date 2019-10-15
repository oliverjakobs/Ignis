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

		template <typename Type>
		void BufferData(std::vector<Type> v, uint usage = GL_STATIC_DRAW);

		void BufferSubData(uint offset, uint size, const void* data);

		void MapBuffer(const void* data, uint size);

		void VertexAttribPointer(uint index, uint size, uint stride, uint offset);
		void VertexAttribIPointer(uint index, uint size, uint stride, uint offset);
		void VertexAttribDivisor(uint index, uint divisor);
	};

	template<typename Type>
	inline void ArrayBuffer::BufferData(std::vector<Type> v, uint usage)
	{
		BufferData(sizeof(v[0]) * v.size(), &v[0], usage);
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