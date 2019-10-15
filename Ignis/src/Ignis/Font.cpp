#include "Font.h"

#define STB_TRUETYPE_IMPLEMENTATION  // force following include to generate implementation
#include "Ignis/Packages/stb_truetype.h"

#include "Obelisk/Obelisk.h"

namespace ignis
{
	Font::Font(const std::string& path, float size)
		: m_chardata(nullptr), m_texture(nullptr)
	{
		m_fontData.FirstChar = 32;
		m_fontData.NumChars = 96; // ASCII 32..126 is 95 glyphs

		m_fontData.BitmapWidth = 512;
		m_fontData.BitmapHeight = 512;

		FILE* file = fopen(path.c_str(), "rb");

		if (file == nullptr)
		{
			DEBUG_ERROR("Failed to load font: {0}", path);
			return;
		}

		// obtain file size:
		fseek(file, 0, SEEK_END);
		uint buffer_size = ftell(file);
		rewind(file);

		// allocate memory to contain the whole file:
		byte* buffer = (byte*)malloc(sizeof(byte) * buffer_size);
		if (buffer == nullptr)
		{
			DEBUG_ERROR("Failed to allocate memory for font: {0}", path);
			fclose(file);
			return;
		}

		if (fread(buffer, sizeof(byte), buffer_size, file) != buffer_size)
		{
			DEBUG_ERROR("Failed to read font: {0}", path);
			fclose(file);
			free(buffer);
			return;
		}

		fclose(file);

		m_chardata = (stbtt_bakedchar*)malloc(sizeof(stbtt_bakedchar) * m_fontData.NumChars);

		// load bitmap
		byte* bitmap = (byte*)malloc(sizeof(byte) * m_fontData.BitmapWidth * m_fontData.BitmapHeight);
		stbtt_BakeFontBitmap(buffer, 0, size, bitmap, m_fontData.BitmapWidth, m_fontData.BitmapHeight, m_fontData.FirstChar, m_fontData.NumChars, m_chardata); // no guarantee this fits!

		TextureConfig config = DEFAULT_CONFIG;
		config.INTERAL_FORMAT = GL_RED;
		config.FORMAT = GL_RED;

		m_texture = new Texture(bitmap, m_fontData.BitmapWidth, m_fontData.BitmapHeight, config);

		free(buffer);
		free(bitmap);

		// set up vertex array
		m_vao.Bind();
		m_vbo.BufferData(sizeof(float) * 4 * 4, nullptr, GL_DYNAMIC_DRAW);
		m_vbo.VertexAttribPointer(0, 4, 4 * sizeof(float), 0);
	}

	Font::~Font()
	{
		SAFE_DELETE(m_texture);

		free(m_chardata);
	}

	void Font::Bind()
	{
		m_texture->Bind();
		m_vao.Bind();
	}

	void Font::Unbind()
	{
		m_texture->Unbind();
		m_vao.Unbind();
	}

	bool Font::LoadCharQuad(char c, float* x, float* y)
	{
		if ((uint)c >= m_fontData.FirstChar && (uint)c < m_fontData.FirstChar + m_fontData.NumChars)
		{
			stbtt_aligned_quad q;
			stbtt_GetBakedQuad(m_chardata, m_fontData.BitmapWidth, m_fontData.BitmapHeight, c - m_fontData.FirstChar, x, y, &q, 1);

			float vertices[]
			{
				q.x0, q.y0, q.s0, q.t0,
				q.x0, q.y1, q.s0, q.t1,
				q.x1, q.y1, q.s1, q.t1,
				q.x1, q.y0, q.s1, q.t0
			};

			// Update content of VBO memory
			m_vbo.BufferSubData(0, sizeof(vertices), vertices);

			return true;
		}

		return false;
	}
}