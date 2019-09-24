#pragma once

#include <glad/glad.h>

#include "Types.h"

namespace ignis
{
	struct TextureConfig
	{
		int INTERAL_FORMAT;
		uint FORMAT;

		uint TYPE;

		int MIN_FILTER;
		int MAG_FILTER;

		int WRAP_S;
		int WRAP_T;
	};

	constexpr TextureConfig DEFAULT_CONFIG{ GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, GL_LINEAR, GL_NEAREST, GL_REPEAT, GL_REPEAT };

	struct Texture
	{
		uint id;

		int width;
		int height;
		int bpp;

		// functions
		Texture(const std::string& path, bool flipOnLoad = true, TextureConfig config = DEFAULT_CONFIG);
		Texture(int width, int height, TextureConfig config = DEFAULT_CONFIG);
		Texture(byte* bitmap, int width, int height, TextureConfig config = DEFAULT_CONFIG);
		~Texture();

		static unsigned int CreateTexture(byte* pixels, int width, int height, TextureConfig config);

		void Bind(uint slot = 0);
		void Unbind();
	};
}