#include "Renderer.h"

void Renderer::RenderTexture(Texture& tex, glm::mat4 proj, glm::mat4 view, glm::mat4 model, Shader& shader, int first, uint count)
{
	shader.Use();

	shader.SetUniformMat4("projection", proj);
	shader.SetUniformMat4("view", view);
	shader.SetUniformMat4("model", model);

	tex.Bind();
	glDrawArrays(GL_TRIANGLES, first, count);
}

void Renderer::RenderTexture(Texture& tex, glm::mat4 proj, glm::mat4 view, glm::mat4 model, Shader& shader, std::vector<uint> indices)
{
	shader.Use();

	shader.SetUniformMat4("projection", proj);
	shader.SetUniformMat4("view", view);
	shader.SetUniformMat4("model", model);

	tex.Bind();
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data());
	tex.Unbind();
}

void Renderer::RenderTextureInstanced(Texture& tex, uint instances, glm::mat4 proj, glm::mat4 view, glm::mat4 model, Shader& shader, std::vector<uint> indices)
{
	shader.Use();

	shader.SetUniformMat4("projection", proj);
	shader.SetUniformMat4("view", view);
	shader.SetUniformMat4("model", model);

	tex.Bind();
	glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data(), instances);
	tex.Unbind();
}

void Renderer::RenderMesh(Mesh& mesh, Texture& tex, glm::mat4 proj, glm::mat4 view, glm::mat4 model, Shader& shader)
{
	shader.Use();

	shader.SetUniformMat4("projection", proj);
	shader.SetUniformMat4("view", view);
	shader.SetUniformMat4("model", model);

	tex.Bind();

	mesh.VAO().Bind();
	glDrawElementsBaseVertex(GL_TRIANGLES, mesh.NumIndices(), GL_UNSIGNED_INT, 0, 0);
	mesh.VAO().Unbind();
	
	tex.Unbind();
}
