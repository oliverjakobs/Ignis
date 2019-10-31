#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "Ignis/Packages/stb_image.h"

#include "Ignis/Callback.h"

namespace ignis
{
	Texture::Texture(const std::string& path, bool flipOnLoad, TextureConfig config)
		: m_width(0), m_height(0), m_activeSlot(0)
	{
		stbi_set_flip_vertically_on_load(flipOnLoad);

		int bpp = 0;

		byte* pixels = stbi_load(path.c_str(), &m_width, &m_height, &bpp, 4);

		if (pixels)
		{
			m_name = CreateTexture(pixels, m_width, m_height, config);
			stbi_image_free(pixels);
		}
		else
		{
			_ignisErrorCallback(ignisErrorLevel::Error, "[Tex] Failed to load texture(" + path + "): " + stbi_failure_reason());
			m_name = 0;
		}
	}

	Texture::Texture(int width, int height, TextureConfig config)
		: m_width(width), m_height(height), m_activeSlot(0)
	{
		m_name = CreateTexture(nullptr, width, height, config);
	}

	Texture::Texture(byte* bitmap, int width, int height, TextureConfig config)
		: m_width(width), m_height(height), m_activeSlot(0)
	{
		m_name = CreateTexture(bitmap, width, height, config);
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &m_name);
		m_name = 0;
	}

	void Texture::Bind(uint slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_name);
		m_activeSlot = slot;
	}

	void Texture::Unbind()
	{
		glActiveTexture(GL_TEXTURE0 + m_activeSlot);
		glBindTexture(GL_TEXTURE_2D, 0);
		m_activeSlot = 0;
	}

	uint CreateTexture(byte* pixels, int width, int height, TextureConfig config)
	{
		uint name;
		glGenTextures(1, &name);
		glBindTexture(GL_TEXTURE_2D, name);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, config.WRAP_S);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, config.WRAP_T);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, config.MIN_FILTER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, config.MAG_FILTER);

		glTexImage2D(GL_TEXTURE_2D, 0, config.INTERAL_FORMAT, width, height, 0, config.FORMAT, config.TYPE, pixels);
		glGenerateMipmap(GL_TEXTURE_2D);

		return name;
	}
}