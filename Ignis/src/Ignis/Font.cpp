#include "Font.h"

#define STB_TRUETYPE_IMPLEMENTATION  // force following include to generate implementation
#include "Ignis/Packages/stb_truetype.h"

#include "Obelisk/Obelisk.h"

namespace ignis
{
	Font::Font(const char* path, float size)
		: m_texture(nullptr)
	{
		m_fontData.FirstChar = 32;
		m_fontData.NumChars = 96; // ASCII 32..126 is 95 glyphs

		m_fontData.BitmapWidth = 512;
		m_fontData.BitmapHeight = 512;

		std::ifstream input(path, std::ios::binary);
		std::vector<byte> buffer(std::istreambuf_iterator<char>(input), {});

		m_fontData.CharData = (stbtt_bakedchar*)malloc(sizeof(stbtt_bakedchar) * m_fontData.NumChars);

		// load bitmap
		TextureConfig config = DEFAULT_CONFIG;
		config.INTERAL_FORMAT = GL_RED;
		config.FORMAT = GL_RED;

		byte* bitmap = (byte*)malloc(sizeof(byte) * m_fontData.BitmapWidth * m_fontData.BitmapHeight);
		stbtt_BakeFontBitmap(buffer.data(), 0, size, bitmap, m_fontData.BitmapWidth, m_fontData.BitmapHeight, m_fontData.FirstChar, m_fontData.NumChars, m_fontData.CharData); // no guarantee this fits!

		m_texture = new Texture(bitmap, m_fontData.BitmapWidth, m_fontData.BitmapHeight, config);

		free(bitmap);

		// set up vertex array
		m_vertexArray.Bind();
		auto vbo = m_vertexArray.AddArrayBuffer(sizeof(float) * 4 * 4, nullptr, GL_DYNAMIC_DRAW);
		vbo->VertexAttribPointer(0, 4, 4 * sizeof(float), 0);

		m_vertexArray.LoadElementBuffer({ 0,1,2,2,3,0 }, GL_STATIC_DRAW);
	}

	Font::~Font()
	{
		SAFE_DELETE(m_texture);

		free(m_fontData.CharData);
	}

	void Font::Bind()
	{
		m_texture->Bind();
		m_vertexArray.Bind();
	}

	void Font::Unbind()
	{
		m_texture->Unbind();
		m_vertexArray.Unbind();
	}

	bool Font::LoadCharQuad(char c, float* x, float* y)
	{
		if ((uint)c >= m_fontData.FirstChar && (uint)c < m_fontData.FirstChar + m_fontData.NumChars)
		{
			stbtt_aligned_quad q;
			stbtt_GetBakedQuad(m_fontData.CharData, m_fontData.BitmapWidth, m_fontData.BitmapHeight, c - m_fontData.FirstChar, x, y, &q, 1);

			float vertices[]
			{
				q.x0, q.y0, q.s0, q.t0,
				q.x0, q.y1, q.s0, q.t1,
				q.x1, q.y1, q.s1, q.t1,
				q.x1, q.y0, q.s1, q.t0
			};

			// Update content of VBO memory
			m_vertexArray.GetArrayBuffer(0)->BufferSubData(0, sizeof(vertices), vertices);

			return true;
		}

		return false;
	}
}