#include "Mesh.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "Core/Debugger.h"

Mesh::Mesh(const std::string& filename)
{
	//Load(OBJModel(filename).ToIndexedModel());

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string warn;
	std::string err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str()))
	{
		DEBUG_ERROR("Failed to load obj: {0}", filename);
		return;
	}

	if (!warn.empty())
		DEBUG_WARN("{0}", warn);

	if (!err.empty())
		DEBUG_ERROR("{0}", err);


	IndexedModel model;

	for (const auto& shape : shapes) 
	{
		for (const auto& index : shape.mesh.indices) 
		{
			float vx = attrib.vertices[3 * index.vertex_index + 0];
			float vy = attrib.vertices[3 * index.vertex_index + 1];
			float vz = attrib.vertices[3 * index.vertex_index + 2];

			float nx = attrib.normals[3 * index.normal_index + 0];
			float ny = attrib.normals[3 * index.normal_index + 1];
			float nz = attrib.normals[3 * index.normal_index + 2];

			float tx = attrib.texcoords[2 * index.texcoord_index + 0];
			float ty = attrib.texcoords[2 * index.texcoord_index + 1];

			model.Positions.push_back(glm::vec3(vx, vy, vz));
			model.Normals.push_back(glm::vec3(nx, ny, nz));
			model.TexCoords.push_back(glm::vec2(tx, ty));

			model.Indices.push_back(model.Indices.size());
		}
	}

	Load(model);
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint> indices)
{
	IndexedModel model;

	for(auto& vertex : vertices)
	{
		model.Positions.push_back(vertex.Position);
		model.TexCoords.push_back(vertex.TexCoord);
		model.Normals.push_back(vertex.Normal);
	}
	
	model.Indices = indices;

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
