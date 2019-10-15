#include "Buffer.h"

namespace ignis
{
	Buffer::Buffer(uint target)
		: Target(target)
	{
		glGenBuffers(1, &Name);
		glBindBuffer(Target, Name);
	}

	Buffer::~Buffer()
	{
		glDeleteBuffers(1, &Name);
		glBindBuffer(Target, 0);
	}

	void Buffer::Bind() const
	{
		glBindBuffer(Target, Name);
	}

	void Buffer::Unbind() const
	{
		glBindBuffer(Target, 0);
	}

	ArrayBuffer::ArrayBuffer() : Buffer(GL_ARRAY_BUFFER) { }

	void ArrayBuffer::BufferData(uint size, const void* data, uint usage)
	{
		Bind();
		glBufferData(Target, size, data, (GLenum)usage);
	}

	void ArrayBuffer::BufferSubData(uint offset, uint size, const void* data)
	{
		Bind();
		glBufferSubData(Target, offset, size, data);
	}

	void ArrayBuffer::MapBuffer(const void* data, uint size)
	{
		Bind();

		void* ptr = glMapBuffer(Target, GL_WRITE_ONLY);

		memcpy(ptr, data, size);

		glUnmapBuffer(Target);
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

	ElementBuffer::ElementBuffer() : Buffer(GL_ELEMENT_ARRAY_BUFFER), Count(0) { }

	void ElementBuffer::BufferData(uint count, const uint* data, uint usage)
	{
		Bind();
		glBufferData(Target, count * sizeof(uint), data, (GLenum)usage);
		Count = count;
	}

	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &Name);
		glBindVertexArray(Name);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &Name);
		glBindVertexArray(0);
	}

	void VertexArray::Bind() const
	{
		glBindVertexArray(Name);
	}

	void VertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	TextureBuffer::TextureBuffer(uint format, uint buffer) : Format(format)
	{
		glGenTextures(1, &Texture);
		glBindTexture(GL_TEXTURE_BUFFER, Texture);
		glTexBuffer(GL_TEXTURE_BUFFER, Format, buffer);
	}

	TextureBuffer::~TextureBuffer()
	{
		glDeleteTextures(1, &Texture);
	}

	void TextureBuffer::BindImageTexture(uint unit, uint access)
	{
		glBindImageTexture(unit, Texture, 0, GL_FALSE, 0, access, Format);
	}

	RenderBuffer::RenderBuffer()
	{
		glGenRenderbuffers(1, &Name);
	}

	RenderBuffer::~RenderBuffer()
	{
		glDeleteRenderbuffers(1, &Name);
	}

	void RenderBuffer::Bind() const
	{
		glBindRenderbuffer(GL_RENDERBUFFER, Name);
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
}