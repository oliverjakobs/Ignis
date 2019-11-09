#include "TextureAtlas.h"

namespace ignis
{
	TextureAtlas::TextureAtlas(int width, int height, GLuint rows, GLuint columns, TextureConfig config) : Texture(width, height, config), m_rows(rows), m_columns(columns) { }

	TextureAtlas::TextureAtlas(const std::string& path, GLuint rows, GLuint columns, TextureConfig config) : Texture(path, true, config), m_rows(rows), m_columns(columns) { }
}
