#include "VertexArray.h"

namespace ignis
{
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