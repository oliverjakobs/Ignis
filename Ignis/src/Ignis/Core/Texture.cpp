#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "Ignis/Packages/stb_image.h"

namespace ignis
{
	Texture::Texture(int width, int height, TextureConfig config)
		: m_width(width), m_height(height), m_activeSlot(0), m_config(config)
	{
		m_name = CreateTexture2D(width, height, nullptr, config);
	}

	Texture::Texture(int width, int height, void* pixels, TextureConfig config)
		: m_width(width), m_height(height), m_activeSlot(0), m_config(config)
	{
		m_name = CreateTexture2D(width, height, pixels, config);
	}

	Texture::Texture(const std::string& path, bool flipOnLoad, TextureConfig config)
		: m_width(0), m_height(0), m_activeSlot(0), m_config(config)
	{
		stbi_set_flip_vertically_on_load(flipOnLoad);

		int bpp = 0;

		byte* pixels = stbi_load(path.c_str(), &m_width, &m_height, &bpp, 4);

		// check if bpp and format matches
		if (bpp == 4)
		{
			if (config.FORMAT != GL_RGBA || config.INTERAL_FORMAT != GL_RGBA8)
				_ignisErrorCallback(ignisErrorLevel::Warn, "[Tex] Format mismatch for " + path);
		}
		else if (bpp == 3)
		{
			if (config.FORMAT != GL_RGB || config.INTERAL_FORMAT != GL_RGB8)
				_ignisErrorCallback(ignisErrorLevel::Warn, "[Tex] Format mismatch for " + path);
		}

		if (pixels)
		{
			m_name = CreateTexture2D(m_width, m_height, pixels, config);
			stbi_image_free(pixels);
		}
		else
		{
			_ignisErrorCallback(ignisErrorLevel::Error, "[Tex] Failed to load texture(" + path + "): " + stbi_failure_reason());
			m_name = 0;
		}
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &m_name);
		m_name = 0;
	}

	void Texture::Bind(uint slot)
	{
		glBindTextureUnit(slot, m_name);
		m_activeSlot = slot;
	}

	void Texture::Unbind()
	{
		glBindTextureUnit(m_activeSlot, 0);
		m_activeSlot = 0;
	}

	uint CreateTexture2D(int width, int height, void* pixels, TextureConfig config)
	{
		uint name;
		glGenTextures(1, &name);
		glBindTexture(GL_TEXTURE_2D, name);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, config.WRAP_S);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, config.WRAP_T);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, config.MIN_FILTER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, config.MAG_FILTER);

		glTexImage2D(GL_TEXTURE_2D, 0, config.INTERAL_FORMAT, width, height, 0, config.FORMAT, GL_UNSIGNED_BYTE, pixels);
		glGenerateMipmap(GL_TEXTURE_2D);

		return name;
	}
}