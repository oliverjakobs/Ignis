#include "Framebuffer.h"

#include "Debugger.h"

#include "Renderer.h"

FrameBuffer::FrameBuffer(int w, int h)
	: m_width(w), m_height(h)
{
	float vertices[]
	{
		// normal
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	};

	// create vao
	m_vao.Bind();
	m_vao.GenVertexBuffer();
	m_vao.SetVertexBufferData(sizeof(vertices), vertices, GL_STATIC_DRAW);
	m_vao.SetVertexAttribPointer(0, 2, 4, 0);
	m_vao.SetVertexAttribPointer(1, 2, 4, 2);

	// Set up framebuffer
	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

	// Create texture for the framebuffer to render to
	m_texture = unique_ptr<Texture>(new Texture(m_width, m_height));
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture->id, 0);

	// TODO: assert
	// check if framebuffer is completed and unbind it
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		DEBUG_WARN("Framebuffer is not complete!");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	m_view = glm::ortho(0.0f, (float)w, 0.0f, (float)h);

	m_model = glm::translate(glm::mat4(), glm::vec3(m_width / 2.0f, m_height / 2.0f, 0.0f));
	m_model = glm::scale(m_model, glm::vec3(m_width / 2.0f, m_height / 2.0f, 0.0f));
}

FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &m_fbo);
}

void FrameBuffer::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

	glViewport(0, 0, m_width, m_height);
}

void FrameBuffer::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, (GLsizei)Renderer::GetView().w, (GLsizei)Renderer::GetView().h);
}

void FrameBuffer::render(const std::string& sName) const
{
	Shader* shader = ResourceManager::GetShader(sName);

	if (shader != nullptr)
	{
		shader->use();
		shader->setUniform2f("uFramePos", glm::vec2());
		shader->setUniformMat4("projection", glm::mat4());
		shader->setUniformMat4("view", Renderer::GetViewMat());
		shader->setUniformMat4("model", m_model);
	}

	m_vao.Bind();

	m_texture->Bind();

	glDrawArrays(GL_TRIANGLES, 0, 6);
}

glm::mat4 FrameBuffer::getView() const
{
	return m_view;
}
