#pragma once

#include <glad/glad.h>

#include "Types.h"

namespace ignis
{
	struct TextureConfig
	{
		uint INTERAL_FORMAT;
		uint FORMAT;

		uint MIN_FILTER;
		uint MAG_FILTER;

		uint WRAP_S;
		uint WRAP_T;
	};

	constexpr TextureConfig DEFAULT_CONFIG{ GL_RGBA8, GL_RGBA, GL_LINEAR, GL_NEAREST, GL_REPEAT, GL_REPEAT };

	struct Texture
	{
	private:
		uint m_name;

		int m_width;
		int m_height;

		uint m_activeSlot;

		TextureConfig m_config;

	public:
		Texture(int width, int height, TextureConfig config = DEFAULT_CONFIG);
		Texture(int width, int height, void* pixels, TextureConfig config = DEFAULT_CONFIG);
		Texture(const std::string& path, bool flipOnLoad = true, TextureConfig config = DEFAULT_CONFIG);
		~Texture();

		void Bind(uint slot = 0);
		void Unbind();

		const uint GetName() const { return m_name; }

		const int GetWidth() const { return m_width; }
		const int GetHeight() const { return m_height; }
	};

	uint CreateTexture2D(int width, int height, void* pixels, TextureConfig config);
}