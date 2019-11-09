#pragma once

#include <glad/glad.h>

#include "Api.h"

namespace ignis
{
	struct TextureConfig
	{
		GLint INTERAL_FORMAT;
		GLenum FORMAT;

		GLint MIN_FILTER;
		GLint MAG_FILTER;

		GLint WRAP_S;
		GLint WRAP_T;
	};

	constexpr TextureConfig DEFAULT_CONFIG{ GL_RGBA8, GL_RGBA, GL_LINEAR, GL_NEAREST, GL_REPEAT, GL_REPEAT };

	struct Texture
	{
	private:
		GLuint m_name;

		int m_width;
		int m_height;

		GLuint m_activeSlot;

		TextureConfig m_config;

	public:
		Texture(int width, int height, TextureConfig config = DEFAULT_CONFIG);
		Texture(int width, int height, void* pixels, TextureConfig config = DEFAULT_CONFIG);
		Texture(const std::string& path, bool flipOnLoad = true, TextureConfig config = DEFAULT_CONFIG);
		~Texture();

		void Bind(GLuint slot = 0);
		void Unbind();

		const GLuint GetName() const { return m_name; }

		const int GetWidth() const { return m_width; }
		const int GetHeight() const { return m_height; }
	};

	GLuint CreateTexture2D(int width, int height, void* pixels, TextureConfig config);
}