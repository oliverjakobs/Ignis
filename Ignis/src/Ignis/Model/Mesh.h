#pragma once

#include "Material.h"
#include "Ignis/Core/VertexArray.h"

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
		VertexArray m_vao;
		ArrayBuffer m_vbo;
		ElementBuffer m_ibo;

	public:
		static Mesh LoadFromFile(const std::string& filename, const std::string& mtldir, Material* mtl);

		Mesh(std::vector<Vertex> vertices);
		virtual ~Mesh();

		// disable copying
		Mesh(const Mesh& mesh) = delete;
		void operator=(const Mesh& mesh) = delete;

		VertexArray& VAO();
		uint ElementCount();
	};
}