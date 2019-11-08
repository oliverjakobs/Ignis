#pragma once

#include <glad/glad.h>

#include "Api.h"

namespace ignis
{
	class Buffer
	{
	protected:
		uint m_name;
		uint m_target;

	public:
		Buffer(uint target);
		virtual ~Buffer();

		void Bind() const;
		void Unbind() const;

		const uint GetName() const { return m_name; }
		const uint GetTarget() const { return m_target; }
	};

	class ArrayBuffer : public Buffer
	{
	public:
		ArrayBuffer();
		ArrayBuffer(uint size, const void* data, uint usage);

		void BufferData(uint size, const void* data, uint usage);
		void BufferSubData(uint offset, uint size, const void* data);

		void VertexAttribPointer(uint index, uint size, bool normalized, uint stride, uint offset);
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

	class ElementBuffer : public Buffer
	{
	private:
		uint m_count;

	public:
		ElementBuffer();
		ElementBuffer(uint count, const uint* data, uint usage);

		void BufferData(uint count, const uint* data, uint usage);

		const uint GetCount() const { return m_count; }
	};

	// Advanced buffers
	class TextureBuffer
	{
	private:
		uint m_texture;
		uint m_format;

	public:
		TextureBuffer(uint format, uint buffer);
		~TextureBuffer();

		void BindImageTexture(uint unit, uint access);

		const uint GetTexture() const { return m_texture; }
		const uint GetFormat() const { return m_format; }
	};

	class RenderBuffer
	{
	private:
		uint m_name;

	public:
		RenderBuffer();
		~RenderBuffer();

		void Bind() const;
		void Unbind() const;

		void RenderbufferStorage(uint format, int width, int height);

		const uint GetName() const { return m_name; }
	};

	// VertexArray
	static uint GetOpenGLTypeSize(uint type)
	{
		switch (type)
		{
		case GL_FLOAT:	return sizeof(float);
		case GL_INT:	return sizeof(int);
		case GL_UNSIGNED_INT: return sizeof(uint);
		default: return 0;
		}
	}

	struct BufferElement
	{
		uint Type;
		uint Count;
		uint Offset;
		bool Normalized;

		BufferElement() : Type(GL_UNSIGNED_INT), Count(0), Offset(0), Normalized(false) { }
		BufferElement(uint type, uint count, bool normalized = false) : Type(type), Count(count), Offset(0), Normalized(normalized) { }
	};

	class BufferLayout
	{
	private:
		std::vector<BufferElement> m_elements;
		uint m_stride = 0;

	public:
		BufferLayout() {}

		BufferLayout(const std::initializer_list<BufferElement>& elements)
			: m_elements(elements)
		{
			// Calculate offsets and stride
			uint offset = 0;
			m_stride = 0;
			for (auto& element : m_elements)
			{
				element.Offset = offset;
				uint size = GetOpenGLTypeSize(element.Type) * element.Count;

				offset += size;
				m_stride += size;
			}
		}

		inline uint GetStride() const { return m_stride; }
		inline const std::vector<BufferElement>& GetElements() const { return m_elements; }

		std::vector<BufferElement>::iterator begin() { return m_elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_elements.end(); }
	};

	class VertexArray
	{
	private:
		uint m_name;

		uint m_vertexAttribIndex;

		std::vector<std::shared_ptr<ArrayBuffer>> m_arrayBuffers;
		std::shared_ptr<ElementBuffer> m_elementBuffer;

	public:
		VertexArray();
		~VertexArray();

		void Bind() const;
		void Unbind() const;

		void AddArrayBuffer(const std::shared_ptr<ArrayBuffer>& buffer);
		void AddArrayBuffer(const std::shared_ptr<ArrayBuffer>& buffer, const BufferLayout& layout);
		void LoadElementBuffer(std::vector<uint> indices, uint usage);

		const uint GetName() const { return m_name; }
		const uint GetElementCount() const { return (m_elementBuffer ? m_elementBuffer->GetCount() : 0); }

		const std::vector<std::shared_ptr<ArrayBuffer>>& GetArrayBuffers() const { return m_arrayBuffers; }
		const std::shared_ptr<ElementBuffer>& GetElementBuffer() const { return m_elementBuffer; }

		std::shared_ptr<ArrayBuffer> GetArrayBuffer(uint index) const { return m_arrayBuffers.at(index); }
	};
}