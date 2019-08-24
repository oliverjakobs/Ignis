#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "../Utility.h"

Texture::Texture(std::string path, TextureConfig config)
{
	stbi_set_flip_vertically_on_load(true);

	width = 0;
	height = 0;
	bpp = 0;

	byte* pixels = stbi_load(path.c_str(), &width, &height, &bpp, 4);

	if (pixels)
	{
		id = CreateTexture(pixels, width, height, config);
		stbi_image_free(pixels);
	}
	else
	{
		DEBUG_MESSAGE("Failed to load texture: " << path);
		id = 0;
	}
}

Texture::Texture(int width, int height, TextureConfig config)
	: width(width), height(height)
{
	id = CreateTexture(nullptr, width, height, config);
}

Texture::~Texture()
{
	glDeleteTextures(1, &id);
	id = 0;
}

unsigned int Texture::CreateTexture(byte* pixels, int width, int height, TextureConfig config)
{
	unsigned int id;
	glGenTextures(1, &id);

	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, config.INTERAL_FORMAT, width, height, 0, config.FORMAT, config.TYPE, pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, config.WRAP_S);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, config.WRAP_T);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, config.MIN_FILTER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, config.MAG_FILTER);

	return id;
}

void Texture::Bind(uint slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}