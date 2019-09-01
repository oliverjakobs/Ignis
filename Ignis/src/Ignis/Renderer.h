#pragma once

#include "Core/Texture.h"
#include "Core/Shader.h"

class Renderer
{
public:
	void RenderTexture(Texture* tex, glm::mat4 proj, glm::mat4 view, glm::mat4 model, Shader* shader, int first, uint count);
	void RenderTexture(Texture* tex, glm::mat4 proj, glm::mat4 view, glm::mat4 model, Shader* shader, vector<uint> indices);
	void RenderTextureInstanced(Texture* tex, uint instances, glm::mat4 proj, glm::mat4 view, glm::mat4 model, Shader* shader, vector<uint> indices);

	void RenderToFile();
};