#include "Framebuffer.h"

#include "Core/Utility.h"

namespace ignis
{
	FrameBuffer::FrameBuffer(int w, int h)
		: m_texture(w, h), m_width(w), m_height(h)
	{
		float vertices[] = {
			// positions   // texCoords
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f, 1.0f
		};

		m_vao.Bind();

		m_vao.GenVertexBuffer();

		m_vao.SetVertexBufferData(sizeof(vertices), vertices);
		m_vao.SetVertexAttribPointer(0, 2, 4, 0);
		m_vao.SetVertexAttribPointer(1, 2, 4, 2);

		glGenFramebuffers(1, &m_fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

		// create a color attachment texture
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture.id, 0);

		// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
		unsigned int rbo;
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h); // use a single renderbuffer object for both a depth AND stencil buffer.
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it

		// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			DEBUG_WARN("Framebuffer is not complete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		m_vao.UnbindVertexBuffer();
	}

	FrameBuffer::~FrameBuffer()
	{

	}

	void FrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	}

	void FrameBuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBuffer::BindVAO()
	{
		m_vao.Bind();
	}

	VAO& FrameBuffer::VAO()
	{
		return m_vao;
	}

	Texture& FrameBuffer::Texture()
	{
		return m_texture;
	}
}