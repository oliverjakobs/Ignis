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
			uint FirstChar;
			uint NumChars;

			uint BitmapWidth;
			uint BitmapHeight;

			stbtt_bakedchar* CharData;
		} m_fontData;
		
		Texture* m_texture;

		// buffer
		VertexArray m_vertexArray;
	public:
		Font(const char* path, float size);
		~Font();

		void Bind();
		void Unbind();

		bool LoadCharQuad(char c, float* x, float* y);
	};
}