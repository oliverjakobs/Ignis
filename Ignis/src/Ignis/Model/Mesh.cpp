#include "Mesh.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "Ignis/Packages/tiny_obj_loader.h"

#include "Obelisk/Debugger.h"

namespace ignis
{
	Mesh Mesh::LoadFromFile(const std::string& filename, const std::string& mtldir, Material* mtl)
	{
		OBELISK_TRACE("[Obj] ------------------------------------------------");
		OBELISK_TRACE("[Obj] Reading obj file {0}", filename);

		OBELISK_CHRONO();

		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;

		std::string warn;
		std::string err;

		bool success = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str(), mtldir.c_str());

		if (!warn.empty())
			_ignisErrorCallback(ignisErrorLevel::Warn, warn);

		if (!err.empty())
			_ignisErrorCallback(ignisErrorLevel::Error, err);

		if (!success)
		{
			_ignisErrorCallback(ignisErrorLevel::Error, "[Obj] Failed to load obj: " + filename);
			return Mesh({});
		}

		OBELISK_TRACE("[Obj] Detected:");
		OBELISK_TRACE("[Obj] {0} vertices", (attrib.vertices.size() / 3));
		OBELISK_TRACE("[Obj] {0} normals", (attrib.normals.size() / 3));
		OBELISK_TRACE("[Obj] {0} texture coords", (attrib.texcoords.size() / 2));

		OBELISK_CHRONO_TRACE("[Obj] Parsed obj file in {0}ms");

		std::vector<Vertex> vertices;

		for (const auto& index : shapes[0].mesh.indices)
		{
			Vertex vertex;

			vertex.Position =
			{
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};

			vertex.TexCoord =
			{
				attrib.texcoords[2 * index.texcoord_index + 0],
				attrib.texcoords[2 * index.texcoord_index + 1]
			};

			vertex.Normal =
			{
				attrib.normals[3 * index.normal_index + 0],
				attrib.normals[3 * index.normal_index + 1],
				attrib.normals[3 * index.normal_index + 2]
			};

			vertex.Tangent = glm::vec3();
			vertex.Bitangent = glm::vec3();

			vertices.push_back(vertex);
		}

		OBELISK_CHRONO_TRACE("[Obj] Converted to vertices in {0}ms");

		if (mtl && materials.size() > 0)
		{
			if (!materials[0].diffuse_texname.empty())
				mtl->Diffuse = new Texture(mtldir + materials[0].diffuse_texname);

			if (!materials[0].bump_texname.empty())
				mtl->Normal = new Texture(mtldir + materials[0].bump_texname);

			if (!materials[0].specular_texname.empty())
				mtl->Specular = new Texture(mtldir + materials[0].specular_texname);

			mtl->Shininess = materials[0].shininess;
		}

		OBELISK_CHRONO_TRACE("[Obj] Materials loaded in {0}ms");
		OBELISK_TRACE("[Obj] ------------------------------------------------");
		return Mesh(vertices);
	}

	Mesh::Mesh(std::vector<Vertex> vertices)
	{
		OBELISK_TRACE("[Mesh] Loading mesh with {0} vertices", vertices.size());

		OBELISK_CHRONO();

		for (unsigned int i = 0; i < vertices.size(); i += 3) 
		{
			// Edges of the triangle : postion delta
			glm::vec3 deltaP1 = vertices[i + 1].Position - vertices[i + 0].Position;
			glm::vec3 deltaP2 = vertices[i + 2].Position - vertices[i + 0].Position;

			// texcoord delta
			glm::vec2 deltaT1 = vertices[i + 1].TexCoord - vertices[i + 0].TexCoord;
			glm::vec2 deltaT2 = vertices[i + 2].TexCoord - vertices[i + 0].TexCoord;

			float r = 1.0f / (deltaT1.x * deltaT2.y - deltaT1.y * deltaT2.x);
			glm::vec3 tangent = (deltaP1 * deltaT2.y - deltaP2 * deltaT1.y) * r;
			glm::vec3 bitangent = (deltaP2 * deltaT1.x - deltaP1 * deltaT2.x) * r;

			vertices[i + 0].Tangent = tangent;
			vertices[i + 1].Tangent = tangent;
			vertices[i + 2].Tangent = tangent;

			vertices[i + 0].Bitangent = bitangent;
			vertices[i + 1].Bitangent = bitangent;
			vertices[i + 2].Bitangent = bitangent;
		}

		// create indicies and eliminate duplicates
		std::vector<Vertex> indexedVert;
		std::vector<uint> indices;

		for (auto& v : vertices) 
		{
			int index = -1;
			for (unsigned int i = 0; i < indexedVert.size(); i++)
			{
				if (v == indexedVert[i])
				{
					index = i;
					break;
				}
			}

			if (index > 0)	// A similar vertex is already in the VBO, use it instead !
			{
				// Average the tangents and the bitangents
				indexedVert[index].Tangent += v.Tangent;
				indexedVert[index].Bitangent += v.Bitangent;
				// set index
				indices.push_back((uint)index);
			}
			else // If not, it needs to be added in the output data.
			{
				indexedVert.push_back(v);
				// set index
				indices.push_back(indexedVert.size() - 1);
			}
		}

		auto vbo = std::make_shared<ArrayBuffer>(sizeof(indexedVert[0]) * indexedVert.size(), &indexedVert[0], GL_STATIC_DRAW);

		m_vertexArray.AddArrayBuffer(vbo);

		vbo->VertexAttribPointer(0, 3, false, sizeof(Vertex), 0);
		vbo->VertexAttribPointer(1, 2, false, sizeof(Vertex), offsetof(Vertex, TexCoord));
		vbo->VertexAttribPointer(2, 3, false, sizeof(Vertex), offsetof(Vertex, Normal));
		vbo->VertexAttribPointer(3, 3, false, sizeof(Vertex), offsetof(Vertex, Tangent));
		vbo->VertexAttribPointer(4, 3, false, sizeof(Vertex), offsetof(Vertex, Bitangent));

		m_vertexArray.LoadElementBuffer(indices, GL_STATIC_DRAW);

		OBELISK_CHRONO_TRACE("[Mesh] Done in {0}ms");
	}

	Mesh::~Mesh()
	{

	}

	VertexArray& Mesh::VAO()
	{
		return m_vertexArray;
	}

	uint Mesh::ElementCount()
	{
		return m_vertexArray.GetElementBuffer()->GetCount();
	}
}