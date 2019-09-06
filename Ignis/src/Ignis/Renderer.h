#pragma once

#include "Core/Texture.h"
#include "Core/Shader.h"

#include "Mesh.h"

class Renderer
{
public:
	void RenderTexture(Texture& tex, glm::mat4 proj, glm::mat4 view, glm::mat4 model, Shader& shader, int first, uint count);
	void RenderTexture(Texture& tex, glm::mat4 proj, glm::mat4 view, glm::mat4 model, Shader& shader, std::vector<uint> indices);
	void RenderTextureInstanced(Texture& tex, uint instances, glm::mat4 proj, glm::mat4 view, glm::mat4 model, Shader& shader, std::vector<uint> indices);

	void RenderMesh(Mesh& mesh, Texture& tex, glm::mat4 proj, glm::mat4 view, glm::mat4 model, Shader& shader);

	void RenderToFile();
};