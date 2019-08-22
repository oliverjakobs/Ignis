#pragma once

#include "Shader.h"
#include "Buffer.h"
#include "Texture.h"

class FrameBuffer
{
private:
	unsigned int m_fbo;
	VAO m_vao;

	int m_width;
	int m_height;

	glm::mat4 m_view;
	glm::mat4 m_model;

	unique_ptr<Texture> m_texture;
public:
	FrameBuffer(int w, int h);
	~FrameBuffer();

	void bind();
	void unbind();

	void render(const std::string& shader) const;

	glm::mat4 getView() const;
};