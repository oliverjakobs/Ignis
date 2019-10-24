#pragma once 

#include "Obelisk/Obelisk.h"

#include "Core/Buffer.h"
#include "Core/Texture.h"
#include "Core/Shader.h"

#include "Model/Mesh.h"
#include "Font.h"

// Renderer
#include "Renderer/FontRenderer.h"

namespace ignis
{
	struct Ignis
	{
		static glm::mat4 ScreenMat;

		static bool Init(uint width, uint height);
		static bool LoadGL(bool debug = false);

		static void EnableBlend(uint sfactor, uint dfactor);
		static void DisableBlend();

		static void RenderTexture(Texture& tex, glm::mat4 proj, glm::mat4 view, glm::mat4 model, Shader& shader, int first, uint count);
		static void RenderTexture(Texture& tex, glm::mat4 proj, glm::mat4 view, glm::mat4 model, Shader& shader, std::vector<uint> indices);
		static void RenderTextureInstanced(Texture& tex, uint instances, glm::mat4 proj, glm::mat4 view, glm::mat4 model, Shader& shader, std::vector<uint> indices);

		static void RenderMesh(Mesh& mesh, Texture& tex, glm::mat4 proj, glm::mat4 view, glm::mat4 model, Shader& shader);
		static void RenderMesh(Mesh& mesh, Material& mtl, glm::mat4 proj, glm::mat4 view, glm::mat4 model, Shader& shader);
	};
}