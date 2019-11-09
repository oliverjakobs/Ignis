#include "Buffer.h"

namespace ignis
{
	Buffer::Buffer(GLenum target)
		: m_target(target)
	{
		glGenBuffers(1, &m_name);
		glBindBuffer(m_target, m_name);
	}

	Buffer::~Buffer()
	{
		glDeleteBuffers(1, &m_name);
		glBindBuffer(m_target, 0);
	}

	void Buffer::Bind() const
	{
		glBindBuffer(m_target, m_name);
	}

	void Buffer::Unbind() const
	{
		glBindBuffer(m_target, 0);
	}

	ArrayBuffer::ArrayBuffer() : Buffer(GL_ARRAY_BUFFER) { }

	ArrayBuffer::ArrayBuffer(GLsizeiptr size, const void* data, GLenum usage) : Buffer(GL_ARRAY_BUFFER)
	{
		BufferData(size, data, usage);
	}

	void ArrayBuffer::BufferData(GLsizeiptr size, const void* data, GLenum usage)
	{
		Bind();
		glBufferData(m_target, size, data, usage);
	}

	void ArrayBuffer::BufferSubData(GLintptr offset, GLsizeiptr size, const void* data)
	{
		Bind();
		glBufferSubData(m_target, offset, size, data);
	}

	void* ArrayBuffer::MapBuffer(GLenum access)
	{
		Bind();
		return glMapBuffer(m_target, access);
	}

	void* ArrayBuffer::MapBufferRange(GLintptr offset, GLsizeiptr length, GLbitfield access)
	{
		Bind();
		return glMapBufferRange(m_target, offset, length, access);
	}

	void ArrayBuffer::UnmapBuffer()
	{
		glUnmapBuffer(m_target);
	}

	void ArrayBuffer::VertexAttribPointer(GLuint index, GLint size, GLboolean normalized, GLsizei stride, const void* offset)
	{
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, size, GL_FLOAT, normalized ? GL_TRUE : GL_FALSE, stride, offset);
	}

	void ArrayBuffer::VertexAttribIPointer(GLuint index, GLint size, GLsizei stride, const void* offset)
	{
		glEnableVertexAttribArray(index);
		glVertexAttribIPointer(index, size, GL_UNSIGNED_INT, stride, offset);
	}

	void ArrayBuffer::VertexAttribDivisor(GLuint index, GLuint divisor)
	{
		glVertexAttribDivisor(index, divisor);
	}

	ElementBuffer::ElementBuffer() : Buffer(GL_ELEMENT_ARRAY_BUFFER), m_count(0) { }

	ElementBuffer::ElementBuffer(GLsizei count, const GLuint* data, GLenum usage) : Buffer(GL_ELEMENT_ARRAY_BUFFER), m_count(count)
	{
		BufferData(count, data, usage);
	}

	void ElementBuffer::BufferData(GLsizei count, const GLuint* data, GLenum usage)
	{
		Bind();
		glBufferData(m_target, count * sizeof(GLuint), data, usage);
		m_count = count;
	}

	TextureBuffer::TextureBuffer(GLenum format, GLuint buffer) : m_format(format)
	{
		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_BUFFER, m_texture);
		glTexBuffer(GL_TEXTURE_BUFFER, m_format, buffer);
	}

	TextureBuffer::~TextureBuffer()
	{
		glDeleteTextures(1, &m_texture);
	}

	void TextureBuffer::BindImageTexture(GLuint unit, GLenum access)
	{
		glBindImageTexture(unit, m_texture, 0, GL_FALSE, 0, access, m_format);
	}

	RenderBuffer::RenderBuffer()
	{
		glGenRenderbuffers(1, &m_name);
	}

	RenderBuffer::~RenderBuffer()
	{
		glDeleteRenderbuffers(1, &m_name);
	}

	void RenderBuffer::Bind() const
	{
		glBindRenderbuffer(GL_RENDERBUFFER, m_name);
	}

	void RenderBuffer::Unbind() const
	{
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	void RenderBuffer::RenderbufferStorage(GLenum format, GLsizei width, GLsizei height)
	{
		Bind();
		glRenderbufferStorage(GL_RENDERBUFFER, format, width, height);
	}

	// VertexArray
	VertexArray::VertexArray() : m_vertexAttribIndex(0)
	{
		glGenVertexArrays(1, &m_name);
		glBindVertexArray(m_name);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_name);
		glBindVertexArray(0);
	}

	void VertexArray::Bind() const
	{
		glBindVertexArray(m_name);
	}

	void VertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void VertexArray::AddArrayBuffer(const std::shared_ptr<ArrayBuffer>& buffer)
	{
		Bind();
		buffer->Bind();

		m_arrayBuffers.push_back(buffer);
	}

	void VertexArray::AddArrayBuffer(const std::shared_ptr<ArrayBuffer>& buffer, const BufferLayout& layout)
	{
		Bind();
		buffer->Bind();

		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(m_vertexAttribIndex);
			glVertexAttribPointer(m_vertexAttribIndex, element.Count, element.Type, element.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)(intptr_t)element.Offset);

			m_vertexAttribIndex++;
		}

		m_arrayBuffers.push_back(buffer);
	}

	void VertexArray::LoadElementBuffer(std::vector<GLuint> indices, GLenum usage)
	{
		m_elementBuffer = std::make_shared<ElementBuffer>(indices.size(), indices.data(), usage);
	}
}