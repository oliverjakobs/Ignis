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

		// buffer
		uint GenBuffer(uint type);

		void SetBufferData(uint type, uint size, const void* data, uint usage = GL_STATIC_DRAW);

		template <typename Type>
		void SetBufferData(uint type, std::vector<Type> v, uint usage = GL_STATIC_DRAW);

		void SetBufferSubData(uint type, uint offset, uint size, const void* data);

		void SetVertexAttribPointer(uint index, uint size, uint stride, uint offset);
		void SetVertexAttribIPointer(uint index, uint size, uint stride, uint offset);
		void SetVertexAttribDivisor(uint index, uint divisor);

		void MapBufferData(uint index, const void* data, uint size);

	};

	template<typename Type>
	inline void VAO::SetBufferData(uint type, std::vector<Type> v, uint usage)
	{
		SetBufferData(type, sizeof(v[0]) * v.size(), &v[0]);
	}
}