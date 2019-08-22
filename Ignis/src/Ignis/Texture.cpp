#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Debugger.h"

Texture::Texture(const char* path, uint slot)
	: slot(slot)
{
	stbi_set_flip_vertically_on_load(true);

	width = 0;
	height = 0;
	bpp = 0;

	byte* pixels = stbi_load(path, &width, &height, &bpp, 4);

	if (pixels)
	{
		TextureConfig config;

		config.INTERAL_FORMAT = GL_RGBA8;
		config.FORMAT = GL_RGBA;
		config.TYPE = GL_UNSIGNED_BYTE;
		config.MIN_FILTER = GL_LINEAR;
		config.MAG_FILTER = GL_NEAREST;
		config.WRAP_S = GL_REPEAT;
		config.WRAP_T = GL_REPEAT;

		id = CreateTexture(pixels, width, height, config);
		stbi_image_free(pixels);
	}
	else
	{
		DEBUG_ERROR("Failed to load texture: {0}", path);
		id = 0;
	}
}

Texture::Texture(int width, int height, uint slot)
	: width(width), height(height), slot(slot)
{
	TextureConfig config;

	config.INTERAL_FORMAT = GL_RGBA8;
	config.FORMAT = GL_RGBA;
	config.TYPE = GL_UNSIGNED_BYTE;
	config.MIN_FILTER = GL_LINEAR;
	config.MAG_FILTER = GL_NEAREST;
	config.WRAP_S = GL_REPEAT;
	config.WRAP_T = GL_REPEAT;

	id = CreateTexture(nullptr, width, height, config);
}

Texture::Texture(const char* path, TextureConfig config, uint slot)
	: slot(slot)
{
	stbi_set_flip_vertically_on_load(true);

	width = 0;
	height = 0;
	bpp = 0;

	byte* pixels = stbi_load(path, &width, &height, &bpp, 4);

	if (pixels)
	{
		id = CreateTexture(pixels, width, height, config);
		stbi_image_free(pixels);
	}
	else
	{
		DEBUG_ERROR("Failed to load texture: {0}", path);
		id = 0;
	}
}

Texture::Texture(int width, int height, TextureConfig config, uint slot)
	: width(width), height(height), slot(slot)
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

void Texture::Bind()
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

TextureAtlas::TextureAtlas(const char* path, uint rows, uint columns, uint slot) : Texture(path, slot), rows(rows), columns(columns) { }

TextureAtlas::TextureAtlas(int width, int height, uint rows, uint columns, uint slot) : Texture(width, height, slot), rows(rows), columns(columns) { }

TextureAtlas::TextureAtlas(const char * path, uint rows, uint columns, TextureConfig config, uint slot) : Texture(path, config, slot), rows(rows), columns(columns) { }

TextureAtlas::TextureAtlas(int width, int height, uint rows, uint columns, TextureConfig config, uint slot) : Texture(width, height, config, slot), rows(rows), columns(columns) { }
