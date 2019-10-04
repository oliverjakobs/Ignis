#pragma once 

#include "Core/Buffer.h"
#include "Core/Texture.h"
#include "Core/Shader.h"

#include "Utility/Utility.h"

#include "Model/Mesh.h"
#include "Font.h"

namespace ignis
{
	bool InitIgnis();
	bool LoadGL(void* loadProc, bool debug = false);

	void RenderTexture(Texture& tex, glm::mat4 proj, glm::mat4 view, glm::mat4 model, Shader& shader, int first, uint count);
	void RenderTexture(Texture& tex, glm::mat4 proj, glm::mat4 view, glm::mat4 model, Shader& shader, std::vector<uint> indices);
	void RenderTextureInstanced(Texture& tex, uint instances, glm::mat4 proj, glm::mat4 view, glm::mat4 model, Shader& shader, std::vector<uint> indices);

	void RenderMesh(Mesh& mesh, Texture& tex, glm::mat4 proj, glm::mat4 view, glm::mat4 model, Shader& shader);
	void RenderMesh(Mesh& mesh, Material& mtl, glm::mat4 proj, glm::mat4 view, glm::mat4 model, Shader& shader);

	void RenderText(const std::string& text, float x, float y, Font& font, glm::mat4 proj, Shader& shader);
}