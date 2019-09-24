#pragma once

#include "Material.h"
#include "Core/Buffer.h"

namespace ignis
{
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec2 TexCoord;
		glm::vec3 Normal;
		glm::vec3 Tangent;
		glm::vec3 Bitangent;

		bool operator==(const Vertex& other) const {
			return Position == other.Position && TexCoord == other.TexCoord && Normal == other.Normal;
		}
	};

	struct Mesh
	{
	private:
		VAO m_vao;
		uint m_numIndices;

	public:
		static Mesh LoadFromFile(const std::string& filename, const std::string& mtldir, Material* mtl);

		Mesh(std::vector<Vertex> vertices);
		virtual ~Mesh();

		// disable copying
		Mesh(const Mesh& mesh) = delete;
		void operator=(const Mesh& mesh) = delete;

		VAO& VAO();
		uint NumIndices();
	};
}