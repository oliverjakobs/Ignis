#include "Buffer.h"

namespace ignis
{
	Buffer::Buffer(uint target)
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

	ArrayBuffer::ArrayBuffer(uint size, const void* data, uint usage) : Buffer(GL_ARRAY_BUFFER)
	{
		BufferData(size, data, usage);
	}

	void ArrayBuffer::BufferData(uint size, const void* data, uint usage)
	{
		Bind();
		glBufferData(m_target, size, data, (GLenum)usage);
	}

	void ArrayBuffer::BufferSubData(uint offset, uint size, const void* data)
	{
		Bind();
		glBufferSubData(m_target, offset, size, data);
	}

	void* ArrayBuffer::MapBuffer(uint access)
	{
		Bind();
		return glMapBuffer(m_target, access);
	}

	void* ArrayBuffer::MapBufferRange(uint offset, uint length, uint access)
	{
		Bind();
		return glMapBufferRange(m_target, offset, length, access);
	}

	void ArrayBuffer::UnmapBuffer()
	{
		glUnmapBuffer(m_target);
	}

	void ArrayBuffer::VertexAttribPointer(uint index, uint size, uint stride, uint offset)
	{
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, (void*)offset);
	}

	void ArrayBuffer::VertexAttribIPointer(uint index, uint size, uint stride, uint offset)
	{
		glEnableVertexAttribArray(index);
		glVertexAttribIPointer(index, size, GL_UNSIGNED_INT, stride, (void*)offset);
	}

	void ArrayBuffer::VertexAttribDivisor(uint index, uint divisor)
	{
		glVertexAttribDivisor(index, divisor);
	}

	ElementBuffer::ElementBuffer() : Buffer(GL_ELEMENT_ARRAY_BUFFER), m_count(0) { }

	ElementBuffer::ElementBuffer(uint count, const uint* data, uint usage) : Buffer(GL_ELEMENT_ARRAY_BUFFER), m_count(count)
	{
		BufferData(count, data, usage);
	}

	void ElementBuffer::BufferData(uint count, const uint* data, uint usage)
	{
		Bind();
		glBufferData(m_target, count * sizeof(uint), data, (GLenum)usage);
		m_count = count;
	}

	TextureBuffer::TextureBuffer(uint format, uint buffer) : m_format(format)
	{
		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_BUFFER, m_texture);
		glTexBuffer(GL_TEXTURE_BUFFER, m_format, buffer);
	}

	TextureBuffer::~TextureBuffer()
	{
		glDeleteTextures(1, &m_texture);
	}

	void TextureBuffer::BindImageTexture(uint unit, uint access)
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

	void RenderBuffer::RenderbufferStorage(uint format, int width, int height)
	{
		Bind();
		glRenderbufferStorage(GL_RENDERBUFFER, format, width, height);
	}

	// VertexArray
	VertexArray::VertexArray()
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

	std::shared_ptr<ArrayBuffer> VertexArray::AddArrayBuffer(uint size, const void* data, uint usage)
	{
		std::shared_ptr<ArrayBuffer> buffer = std::make_shared<ArrayBuffer>(size, data, usage);
		m_arrayBuffers.push_back(buffer);

		return buffer;
	}

	void VertexArray::LoadElementBuffer(std::vector<uint> indices, uint usage)
	{
		m_elementBuffer = std::make_shared<ElementBuffer>(indices.size(), indices.data(), usage);
	}
}