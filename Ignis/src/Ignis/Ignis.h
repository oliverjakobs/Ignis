#pragma once

#include "Core/Buffer.h"
#include "Core/Texture.h"
#include "Core/Shader.h"

#include "Model/Mesh.h"
#include "Font.h"

#include "Renderer/RenderState.h"

// Renderer
#include "Renderer/FontRenderer.h"
#include "Renderer/Renderer2D.h"

namespace ignis
{
	bool ignisInit(int width, int height);
	bool ignisLoadGL(bool debug = false);
	void ignisViewport(int x, int y, int w, int h);

	const glm::mat4& ignisScreenMat();

	struct Ignis
	{
		static void RenderTexture(Texture& tex, glm::mat4 proj, glm::mat4 view, glm::mat4 model, Shader& shader, int first, GLsizei count);
		static void RenderTexture(Texture& tex, glm::mat4 proj, glm::mat4 view, glm::mat4 model, Shader& shader, std::vector<GLuint> indices);
		static void RenderTextureInstanced(Texture& tex, GLsizei instances, glm::mat4 proj, glm::mat4 view, glm::mat4 model, Shader& shader, std::vector<GLuint> indices);

		static void RenderMesh(Mesh& mesh, Texture& tex, glm::mat4 proj, glm::mat4 view, glm::mat4 model, Shader& shader);
		static void RenderMesh(Mesh& mesh, Material& mtl, glm::mat4 proj, glm::mat4 view, glm::mat4 model, Shader& shader);
	};
}