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

		for (const auto& index : shapes[0].mesh.indices)
		{
			float vx = attrib.vertices[3 * index.vertex_index + 0];
			float vy = attrib.vertices[3 * index.vertex_index + 1];
			float vz = attrib.vertices[3 * index.vertex_index + 2];

			float nx = attrib.normals[3 * index.normal_index + 0];
			float ny = attrib.normals[3 * index.normal_index + 1];
			float nz = attrib.normals[3 * index.normal_index + 2];

			float tx = attrib.texcoords[2 * index.texcoord_index + 0];
			float ty = attrib.texcoords[2 * index.texcoord_index + 1];

			Vertex vertex;

			vertex.Position = glm::vec3(vx, vy, vz);
			vertex.TexCoord = glm::vec2(tx, ty);
			vertex.Normal = glm::vec3(nx, ny, nz);
			vertex.Tangent = glm::vec3();
			vertex.Bitangent = glm::vec3();

			vertices.push_back(vertex);
			indices.push_back(indices.size());
		}

		for (uint i = 0; i < vertices.size(); i += 3)
		{
			// Shortcuts for vertices
			glm::vec3& p0 = vertices[i + 0].Position;
			glm::vec3& p1 = vertices[i + 1].Position;
			glm::vec3& p2 = vertices[i + 2].Position;

			// Shortcuts for texcoords
			glm::vec2& t0 = vertices[i + 0].TexCoord;
			glm::vec2& t1 = vertices[i + 1].TexCoord;
			glm::vec2& t2 = vertices[i + 2].TexCoord;

			// Edges of the triangle : position delta
			glm::vec3 deltaP1 = p1 - p0;
			glm::vec3 deltaP2 = p2 - p0;

			// texcoord delta
			glm::vec2 deltaT1 = t1 - t0;
			glm::vec2 deltaT2 = t2 - t0;

			float r = 1.0f / (deltaT1.x * deltaT2.y - deltaT1.y * deltaT2.x);

			glm::vec3 tangent = (deltaP1 * deltaT2.y - deltaP2 * deltaT1.y) * r;
			glm::vec3 bitangent = (deltaP2 * deltaT1.x - deltaP1 * deltaT2.x) * r;
			tangent = glm::normalize(tangent);
			bitangent = glm::normalize(bitangent);

			vertices[i + 0].Tangent = tangent;
			vertices[i + 1].Tangent = tangent;
			vertices[i + 2].Tangent = tangent;

			vertices[i + 0].Bitangent = bitangent;
			vertices[i + 1].Bitangent = bitangent;
			vertices[i + 2].Bitangent = bitangent;
		}

		if (mtl && materials.size() > 0)
		{
			if (!materials[0].diffuse_texname.empty())
				mtl->Diffuse = new Texture(mtldir + materials[0].diffuse_texname);

			if (!materials[0].normal_texname.empty())
				mtl->Normal = new Texture(mtldir + materials[0].normal_texname);

			if (!materials[0].specular_texname.empty())
				mtl->Specular = new Texture(mtldir + materials[0].specular_texname);

			mtl->Shininess = materials[0].shininess;
		}

		return Mesh(vertices, indices);
	}

	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint> indices)
	{
		std::vector<glm::vec3> positions;
		std::vector<glm::vec2> texcoords;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec3> tangents;
		std::vector<glm::vec3> bitangents;

		for (auto& vertex : vertices)
		{
			positions.push_back(vertex.Position);
			texcoords.push_back(vertex.TexCoord);
			normals.push_back(vertex.Normal);
			tangents.push_back(vertex.Tangent);
			bitangents.push_back(vertex.Bitangent);
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

		m_vao.GenBuffer(GL_ARRAY_BUFFER);
		m_vao.SetBufferData(GL_ARRAY_BUFFER, sizeof(tangents[0]) * tangents.size(), &tangents[0]);
		m_vao.SetVertexAttribPointer(3, 3, 0, 0);

		m_vao.GenBuffer(GL_ARRAY_BUFFER);
		m_vao.SetBufferData(GL_ARRAY_BUFFER, sizeof(bitangents[0]) * bitangents.size(), &bitangents[0]);
		m_vao.SetVertexAttribPointer(4, 3, 0, 0);

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