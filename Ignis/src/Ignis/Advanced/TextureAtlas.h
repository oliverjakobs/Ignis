#pragma once

#include "Ignis/Core/Texture.h"

namespace ignis
{
	class TextureAtlas : public Texture
	{
	private:
		GLuint m_rows;
		GLuint m_columns;

	public:
		TextureAtlas(int width, int height, GLuint rows, GLuint columns, TextureConfig config = DEFAULT_CONFIG);
		TextureAtlas(const std::string& path, GLuint rows, GLuint columns, TextureConfig config = DEFAULT_CONFIG);

		const GLuint GetRows() const { return m_rows; }
		const GLuint GetColumns() const { return m_columns; }

		const float GetFrameWidth() const;
		const float GetFrameHeight() const;
	};
}