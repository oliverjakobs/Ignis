#pragma once

#include <glad/glad.h>

#include "Types.h"

struct TextureConfig
{
	int INTERAL_FORMAT;
	uint FORMAT;

	uint TYPE;

	int WRAP_S;
	int WRAP_T;

	int MIN_FILTER;
	int MAG_FILTER;
};

struct Texture
{
	uint id;
	uint slot;

	int width;
	int height;
	int bpp;

	// functions
	Texture(const char* path, uint slot = 0);
	Texture(int width, int height, uint slot = 0);
	Texture(const char* path, TextureConfig config, uint slot = 0);
	Texture(int width, int height, TextureConfig config, uint slot = 0);
	~Texture();

	static unsigned int CreateTexture(byte* pixels, int width, int height, TextureConfig config);

	void Bind();
	void Unbind();
};

struct TextureAtlas : Texture
{
	uint rows;
	uint columns;

	TextureAtlas(const char* path, uint rows, uint columns, uint slot = 0);
	TextureAtlas(int width, int height, uint rows, uint columns, uint slot = 0);
	TextureAtlas(const char* path, uint rows, uint columns, TextureConfig config, uint slot = 0);
	TextureAtlas(int width, int height, uint rows, uint columns, TextureConfig config, uint slot = 0);
};