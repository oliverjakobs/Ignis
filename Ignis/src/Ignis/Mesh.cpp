#include "Mesh.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

Mesh::Mesh(const std::string& fileName)
{
	Load(OBJModel(fileName).ToIndexedModel());
}

Mesh::Mesh(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices)
{
    IndexedModel model;

	for(unsigned int i = 0; i < numVertices; i++)
	{
		model.Positions.push_back(vertices[i].Position);
		model.TexCoords.push_back(vertices[i].TexCoord);
		model.Normals.push_back(vertices[i].Normal);
	}
	
	for(unsigned int i = 0; i < numIndices; i++)
        model.Indices.push_back(indices[i]);

	Load(model);
}

Mesh::~Mesh()
{

}

void Mesh::Load(const IndexedModel& model)
{
	m_numIndices = model.Indices.size();

	m_vao.Bind();

	m_vao.GenVertexBuffer();
	m_vao.SetVertexBufferData(sizeof(model.Positions[0]) * model.Positions.size(), &model.Positions[0]);
	m_vao.SetVertexAttribPointer(0, 3, 0, 0);

	m_vao.GenVertexBuffer();
	m_vao.SetVertexBufferData(sizeof(model.TexCoords[0]) * model.TexCoords.size(), &model.TexCoords[0]);
	m_vao.SetVertexAttribPointer(1, 2, 0, 0);

	m_vao.GenVertexBuffer();
	m_vao.SetVertexBufferData(sizeof(model.Normals[0]) * model.Normals.size(), &model.Normals[0]);
	m_vao.SetVertexAttribPointer(2, 3, 0, 0);

	m_vao.GenIndexBuffer();
	m_vao.SetIndexBufferData(sizeof(model.Indices[0]) * model.Indices.size(), &model.Indices[0]);

	glBindVertexArray(0);
}

VAO& Mesh::VAO()
{
	return m_vao;
}

uint Mesh::NumIndices()
{
	return m_numIndices;
}
