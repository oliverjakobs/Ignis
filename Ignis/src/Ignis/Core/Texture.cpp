#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Utility.h"

namespace ignis
{
	Texture::Texture(const std::string& path, bool flipOnLoad, TextureConfig config)
		: m_activeSlot(0)
	{
		DEBUG_TRACE("[Tex] ------------------------------------------------");
		DEBUG_TRACE("[Tex] Loading {0}", path);

		Timer timer;
		timer.Start();

		stbi_set_flip_vertically_on_load(flipOnLoad);

		Width = 0;
		Height = 0;
		BPP = 0;

		byte* pixels = stbi_load(path.c_str(), &Width, &Height, &BPP, 4);

		timer.End();
		DEBUG_TRACE("[Tex] Parsed file in {0}ms", timer.GetDurationMS());

		if (pixels)
		{
			ID = CreateTexture(pixels, Width, Height, config);
			stbi_image_free(pixels);
		}
		else
		{
			DEBUG_ERROR("[Tex] Failed to load texture: {0}", path);
			ID = 0;
		}

		timer.End();

		DEBUG_TRACE("[Tex] Loaded Texture ({0}) in {1}ms", ID, timer.GetDurationMS());
		DEBUG_TRACE("[Tex] Size: {0}x{1} (bpp: {2})", Width, Height, BPP);
	}

	Texture::Texture(int width, int height, TextureConfig config)
		: Width(width), Height(height), m_activeSlot(0)
	{
		ID = CreateTexture(nullptr, width, height, config);
	}

	Texture::Texture(byte* bitmap, int width, int height, TextureConfig config)
		: Width(width), Height(height), m_activeSlot(0)
	{
		ID = CreateTexture(bitmap, width, height, config);
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &ID);
		ID = 0;
	}

	unsigned int Texture::CreateTexture(byte* pixels, int width, int height, TextureConfig config)
	{
		unsigned int id;
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, config.WRAP_S);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, config.WRAP_T);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, config.MIN_FILTER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, config.MAG_FILTER);

		glTexImage2D(GL_TEXTURE_2D, 0, config.INTERAL_FORMAT, width, height, 0, config.FORMAT, config.TYPE, pixels);
		glGenerateMipmap(GL_TEXTURE_2D);

		return id;
	}

	void Texture::Bind(uint slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, ID);
		m_activeSlot = slot;
	}

	void Texture::Unbind()
	{
		glActiveTexture(GL_TEXTURE0 + m_activeSlot);
		glBindTexture(GL_TEXTURE_2D, 0);
		m_activeSlot = 0;
	}
}