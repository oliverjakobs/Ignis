#pragma once

#include <glad/glad.h>

#include "Api.h"

namespace ignis
{
	class Buffer
	{
	protected:
		GLuint m_name;
		GLenum m_target;

	public:
		Buffer(GLenum target);
		virtual ~Buffer();

		void Bind() const;
		void Unbind() const;

		const GLuint GetName() const { return m_name; }
		const GLenum GetTarget() const { return m_target; }
	};

	class ArrayBuffer : public Buffer
	{
	public:
		ArrayBuffer();
		ArrayBuffer(GLsizeiptr size, const void* data, GLenum usage);

		void BufferData(GLsizeiptr size, const void* data, GLenum usage);
		void BufferSubData(GLintptr offset, GLsizeiptr size, const void* data);

		void VertexAttribPointer(GLuint index, GLint size, GLboolean normalized, GLsizei stride, const void* offset);
		void VertexAttribIPointer(GLuint index, GLint size, GLsizei stride, const void* offset);
		void VertexAttribDivisor(GLuint index, GLuint divisor);

		void* MapBuffer(GLenum access);
		void* MapBufferRange(GLintptr offset, GLsizeiptr length, GLbitfield access);

		template <typename Type> Type* MapBuffer(GLenum access);
		template <typename Type> Type* MapBufferRange(GLintptr offset, GLsizeiptr count, GLbitfield access);

		void UnmapBuffer();
	};

	template <typename Type>
	inline Type* ArrayBuffer::MapBuffer(GLenum access)
	{
		return (Type*)MapBuffer(access);
	}

	template <typename Type>
	inline Type* ArrayBuffer::MapBufferRange(GLintptr offset, GLsizeiptr count, GLbitfield access)
	{
		return (Type*)MapBufferRange(offset, count * sizeof(Type), access);
	}

	class ElementBuffer : public Buffer
	{
	private:
		GLsizei m_count;

	public:
		ElementBuffer();
		ElementBuffer(GLsizei count, const GLuint* data, GLenum usage);

		void BufferData(GLsizei count, const GLuint* data, GLenum usage);

		const GLsizei GetCount() const { return m_count; }
	};

	// Advanced buffers
	class TextureBuffer
	{
	private:
		GLuint m_texture;
		GLenum m_format;

	public:
		TextureBuffer(GLenum format, GLuint buffer);
		~TextureBuffer();

		void BindImageTexture(GLuint unit, GLenum access);

		const GLuint GetTexture() const { return m_texture; }
		const GLenum GetFormat() const { return m_format; }
	};

	class RenderBuffer
	{
	private:
		GLuint m_name;

	public:
		RenderBuffer();
		~RenderBuffer();

		void Bind() const;
		void Unbind() const;

		void RenderbufferStorage(GLenum format, GLsizei width, GLsizei height);

		const GLuint GetName() const { return m_name; }
	};

	// VertexArray
	static unsigned int GetOpenGLTypeSize(GLenum type)
	{
		switch (type)
		{
		case GL_FLOAT:	return sizeof(GLfloat);
		case GL_INT:	return sizeof(GLint);
		case GL_UNSIGNED_INT: return sizeof(GLuint);
		default: return 0;
		}
	}

	struct BufferElement
	{
		GLenum Type;
		GLsizei Count;
		GLuint Offset;
		GLboolean Normalized;

		BufferElement() : Type(GL_UNSIGNED_INT), Count(0), Offset(0), Normalized(GL_FALSE) { }
		BufferElement(GLenum type, GLsizei count, GLboolean normalized = GL_FALSE) : Type(type), Count(count), Offset(0), Normalized(normalized) { }
	};

	class BufferLayout
	{
	private:
		std::vector<BufferElement> m_elements;
		GLuint m_stride = 0;

	public:
		BufferLayout() {}

		BufferLayout(const std::initializer_list<BufferElement>& elements)
			: m_elements(elements)
		{
			// Calculate offsets and stride
			GLuint offset = 0;
			m_stride = 0;
			for (auto& element : m_elements)
			{
				element.Offset = offset;
				GLuint size = GetOpenGLTypeSize(element.Type) * element.Count;

				offset += size;
				m_stride += size;
			}
		}

		inline GLuint GetStride() const { return m_stride; }
		inline const std::vector<BufferElement>& GetElements() const { return m_elements; }

		std::vector<BufferElement>::iterator begin() { return m_elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_elements.end(); }
	};

	class VertexArray
	{
	private:
		GLuint m_name;

		GLuint m_vertexAttribIndex;

		std::vector<std::shared_ptr<ArrayBuffer>> m_arrayBuffers;
		std::shared_ptr<ElementBuffer> m_elementBuffer;

	public:
		VertexArray();
		~VertexArray();

		void Bind() const;
		void Unbind() const;

		void AddArrayBuffer(const std::shared_ptr<ArrayBuffer>& buffer);
		void AddArrayBuffer(const std::shared_ptr<ArrayBuffer>& buffer, const BufferLayout& layout);
		void LoadElementBuffer(std::vector<GLuint> indices, GLenum usage);

		const GLuint GetName() const { return m_name; }
		const GLsizei GetElementCount() const { return (m_elementBuffer ? m_elementBuffer->GetCount() : 0); }

		const std::vector<std::shared_ptr<ArrayBuffer>>& GetArrayBuffers() const { return m_arrayBuffers; }
		const std::shared_ptr<ElementBuffer>& GetElementBuffer() const { return m_elementBuffer; }

		std::shared_ptr<ArrayBuffer> GetArrayBuffer(GLsizei index) const { return m_arrayBuffers.at(index); }
	};
}