#pragma once

#include "Texture.h"
#include "Shader.h"

class Renderer
{
public:
	void RenderTexture(Texture* tex, glm::mat4 proj, glm::mat4 view, glm::mat4 model, Shader* shader, std::vector<unsigned int> indices = { 0, 1, 2, 2, 3, 0 });
	void RenderTextureInstanced(Texture* tex, uint instances, glm::mat4 proj, glm::mat4 view, glm::mat4 model, Shader* shader, std::vector<unsigned int> indices = { 0, 1, 2, 2, 3, 0 });
};