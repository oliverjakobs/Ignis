#pragma once

#include "Core/Buffer.h"
#include "Core/Texture.h"

#include "Ignis/Packages/stb_truetype.h"

namespace ignis
{
	class Font
	{
	private:
		struct FontData
		{
			int FirstChar;
			size_t NumChars;

			int BitmapWidth;
			int BitmapHeight;

			stbtt_bakedchar* CharData;
		} m_fontData;
		
		std::unique_ptr<Texture> m_texture;

		// buffer
		VertexArray m_vertexArray;
	public:
		Font(const std::string& path, float size);
		~Font();

		void Bind();
		void Unbind();

		bool LoadCharQuad(char c, float* x, float* y);
	};
}