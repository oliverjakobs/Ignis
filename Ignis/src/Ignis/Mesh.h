#pragma once

#include "Core/Buffer.h"

#include "obj_loader.h"

struct Vertex
{
	glm::vec3 Position;
	glm::vec2 TexCoord;
	glm::vec3 Normal;

	Vertex(const glm::vec3& pos, const glm::vec2& texCoord, const glm::vec3& normal)
		: Position(pos), TexCoord(texCoord), Normal(normal) { }
};

struct Mesh
{
private:
	VAO m_vao;
	uint m_numIndices;

public:
    Mesh(const std::string& fileName);
	Mesh(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices);
	virtual ~Mesh();

    void Load(const IndexedModel& model);

	// disable copying
	Mesh(const Mesh& mesh) = delete;
	void operator=(const Mesh& mesh) = delete;

	VAO& VAO();
	uint NumIndices();
};