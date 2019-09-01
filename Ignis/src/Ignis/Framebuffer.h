#pragma once

#include "Core/Texture.h"
#include "Core/Buffer.h"

class FrameBuffer
{
private:
	uint m_fbo;

	VAO m_vao;
	Texture* m_texture;

	int m_width, m_height;
public:
	FrameBuffer(int w, int h);
	~FrameBuffer();

	void Bind();
	void Unbind();

	VAO& VAO();
	Texture* Texture();
};