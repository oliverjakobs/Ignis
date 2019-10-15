#pragma once

#include "Core/Buffer.h"
#include "Core/Texture.h"

#include "Ignis/Packages/stb_truetype.h"

namespace ignis
{
	class Font
	{
	private:
		uint m_firstchar;
		uint m_numchars;

		uint m_bitmap_width;
		uint m_bitmap_height;

		stbtt_bakedchar* m_chardata;
		
		Texture* m_texture;

		// buffer
		VertexArray m_vao;
		ArrayBuffer m_vbo;
	public:
		Font(const std::string& path, float size);
		~Font();

		void Bind();
		void Unbind();

		bool LoadCharQuad(char c, float* x, float* y);
	};
}