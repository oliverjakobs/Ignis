#pragma once

#include "Core/Texture.h"
#include "Core/Buffer.h"

namespace ignis
{
	class FrameBuffer
	{
	private:
		uint m_id;
		Texture m_texture;

		VertexArray m_vao;
		ArrayBuffer m_vbo;

		int m_width, m_height;
	public:
		FrameBuffer(int w, int h);
		~FrameBuffer();

		void Bind();
		void Unbind();

		void BindVAO();

		VertexArray& VAO();
		Texture& Texture();
	};
}