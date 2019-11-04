#pragma once

#include "Ignis/Camera/OrthographicCamera.h"

#include "Ignis/Core/Buffer.h"
#include "Ignis/Core/Shader.h"
#include "Ignis/Core/Texture.h"

namespace ignis
{
	struct Renderer2D
	{
		static void Init();
		static void Destroy();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static void RenderQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void RenderQuad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<Texture>& texture);
	};
}
