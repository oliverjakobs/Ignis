#include "Mesh.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "Core/Debugger.h"

#include <unordered_map>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

namespace std 
{
	template<> struct hash<ignis::Vertex> 
	{
		size_t operator()(ignis::Vertex const& vertex) const 
		{
			return ((hash<glm::vec3>()(vertex.Position) ^
				(hash<glm::vec3>()(vertex.Normal) << 1)) >> 1) ^
				(hash<glm::vec2>()(vertex.TexCoord) << 1);
		}
	};
}

namespace ignis
{
	Mesh Mesh::LoadFromFile(const std::string& filename, const std::string& mtldir, Material* mtl)
	{
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;

		std::string warn;
		std::string err;

		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str(), mtldir.c_str()))
		{
			DEBUG_ERROR("Failed to load obj: {0}", filename);
			return Mesh({}, {});
		}

		if (!warn.empty())
			DEBUG_WARN("{0}", warn);

		if (!err.empty())
			DEBUG_ERROR("{0}", err);

		std::vector<Vertex> vertices;
		std::vector<uint> indices;

		std::unordered_map<Vertex, uint> uniqueVertices = {};

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

				Vertex vertex = { glm::vec3(vx, vy, vz), glm::vec2(tx, ty), glm::vec3(nx, ny, nz) };

				if (uniqueVertices.count(vertex) == 0)
				{
					uniqueVertices[vertex] = vertices.size();
					vertices.push_back(vertex);
				}

				indices.push_back(uniqueVertices[vertex]);
			}
		}

		if (mtl)
		{
			if (!materials[0].diffuse_texname.empty())
				mtl->Diffuse = new Texture(mtldir + materials[0].diffuse_texname);

			if (!materials[0].normal_texname.empty())
				mtl->Normal = new Texture(mtldir + materials[0].normal_texname);
		}

		return Mesh(vertices, indices);
	}

	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint> indices)
	{
		std::vector<glm::vec3> positions;
		std::vector<glm::vec2> texcoords;
		std::vector<glm::vec3> normals;

		for (auto& vertex : vertices)
		{
			positions.push_back(vertex.Position);
			texcoords.push_back(vertex.TexCoord);
			normals.push_back(vertex.Normal);
		}

		m_numIndices = indices.size();

		m_vao.Bind();

		m_vao.GenBuffer(GL_ARRAY_BUFFER);
		m_vao.SetBufferData(GL_ARRAY_BUFFER, sizeof(positions[0]) * positions.size(), &positions[0]);
		m_vao.SetVertexAttribPointer(0, 3, 0, 0);

		m_vao.GenBuffer(GL_ARRAY_BUFFER);
		m_vao.SetBufferData(GL_ARRAY_BUFFER, sizeof(texcoords[0]) * texcoords.size(), &texcoords[0]);
		m_vao.SetVertexAttribPointer(1, 2, 0, 0);

		m_vao.GenBuffer(GL_ARRAY_BUFFER);
		m_vao.SetBufferData(GL_ARRAY_BUFFER, sizeof(normals[0]) * normals.size(), &normals[0]);
		m_vao.SetVertexAttribPointer(2, 3, 0, 0);

		m_vao.GenBuffer(GL_ELEMENT_ARRAY_BUFFER);
		m_vao.SetBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0]);

		m_vao.Unbind();
	}

	Mesh::~Mesh()
	{

	}

	VAO& Mesh::VAO()
	{
		return m_vao;
	}

	uint Mesh::NumIndices()
	{
		return m_numIndices;
	}
}