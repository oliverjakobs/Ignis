#pragma once

#include "Core/Buffer.h"
#include "Core/Texture.h"
#include "Core/Shader.h"

#include "Model/Mesh.h"
#include "Font.h"

#include "Callback.h"

// Renderer
#include "Renderer/FontRenderer.h"

namespace ignis
{
	bool ignisInit(uint width, uint height);
	bool ignisLoadGL(bool debug = false);

	const glm::mat4& ignisScreenMat();

	struct Ignis
	{
		static void RenderTexture(Texture& tex, glm::mat4 proj, glm::mat4 view, glm::mat4 model, Shader& shader, int first, uint count);
		static void RenderTexture(Texture& tex, glm::mat4 proj, glm::mat4 view, glm::mat4 model, Shader& shader, std::vector<uint> indices);
		static void RenderTextureInstanced(Texture& tex, uint instances, glm::mat4 proj, glm::mat4 view, glm::mat4 model, Shader& shader, std::vector<uint> indices);

		static void RenderMesh(Mesh& mesh, Texture& tex, glm::mat4 proj, glm::mat4 view, glm::mat4 model, Shader& shader);
		static void RenderMesh(Mesh& mesh, Material& mtl, glm::mat4 proj, glm::mat4 view, glm::mat4 model, Shader& shader);
	};
}