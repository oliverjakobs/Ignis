#pragma once

#include "Buffer.h"

namespace ignis
{
	class VertexArray
	{
	private:
		uint m_name;

		std::vector<std::shared_ptr<ArrayBuffer>> m_arrayBuffers;
		std::shared_ptr<ElementBuffer> m_elementBuffer;

	public:
		VertexArray();
		~VertexArray();

		void Bind() const;
		void Unbind() const;

		// TODO: ArrayBuffer Layout?
		std::shared_ptr<ArrayBuffer> AddArrayBuffer(uint size, const void* data, uint usage);
		void LoadElementBuffer(std::vector<uint> indices, uint usage);

		const uint GetName() const { return m_name; }
		const std::vector<std::shared_ptr<ArrayBuffer>>& GetArrayBuffers() const { return m_arrayBuffers; }
		const std::shared_ptr<ElementBuffer>& GetElementBuffer() const { return m_elementBuffer; }

		std::shared_ptr<ArrayBuffer> GetArrayBuffer(uint index) const { return m_arrayBuffers.at(index); }
	};
}