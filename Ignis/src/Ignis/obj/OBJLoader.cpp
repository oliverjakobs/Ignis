#include "OBJLoader.h"

#include <iostream>     // std::cout
#include <fstream>      // std::ifstream
#include <sstream>      // std::istringstream

#include "Ignis/Core/Debugger.h"

bool loadOBJ(const char* path, std::vector<glm::vec3>& out_vertices, std::vector<glm::vec2>& out_uvs, std::vector<glm::vec3>& out_normals)
{
	printf("Loading OBJ file %s...\n", path);

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

	std::ifstream in(path, std::ios::in);
	if (!in)
	{
		std::cerr << "Cannot open " << path << std::endl;
		return false;
	}
	std::string line;
	while (std::getline(in, line))
	{
		//check v for vertices
		if (line.substr(0, 2) == "v ") 
		{
			std::istringstream v(line.substr(2));
			glm::vec3 vert;
			float x, y, z;
			v >> x; v >> y; v >> z;
			vert = glm::vec3(x, y, z);
			vertices.push_back(vert);
		}
		//check for texture co-ordinate
		else if (line.substr(0, 2) == "vt")
		{
			std::istringstream v(line.substr(3));
			glm::vec2 tex;
			float U, V;
			v >> U; v >> V;
			tex = glm::vec2(U, V);
			uvs.push_back(tex);
		}
		//check for normal co-ordinate
		else if (line.substr(0, 2) == "vn")
		{
			std::istringstream v(line.substr(3));
			glm::vec3 normal;
			float x, y, z;
			v >> x; v >> y; v >> z;
			normal = glm::vec3(x, y, z);
			normals.push_back(normal);
		}
		//check for faces
		else if (line.substr(0, 2) == "f ") 
		{
			unsigned int v[4];	//to store mesh index
			unsigned int uv[4];	//to store texture index
			unsigned int nv[4];	//to store normal index

			const char* chh = line.c_str();
			//int matches = sscanf(chh, "f %i/%i/%i %i/%i/%i %i/%i/%i", &v[0], &uv[0], &nv[0], &v[1], &uv[1], &nv[1], &v[2], &uv[2], &nv[2]);
			int matches = sscanf(chh, "f %i/%i/%i %i/%i/%i %i/%i/%i %i/%i/%i", &v[0], &uv[0], &nv[0], &v[1], &uv[1], &nv[1], &v[2], &uv[2], &nv[2], &v[3], &uv[3], &nv[3]);
			//DEBUG_INFO("Matches {0}", matches);

			vertexIndices.push_back(v[0]);
			vertexIndices.push_back(v[1]);
			vertexIndices.push_back(v[2]);

			vertexIndices.push_back(v[2]);
			vertexIndices.push_back(v[3]);
			vertexIndices.push_back(v[0]);

			uvIndices.push_back(uv[0]);
			uvIndices.push_back(uv[1]);
			uvIndices.push_back(uv[2]);

			uvIndices.push_back(uv[2]);
			uvIndices.push_back(uv[3]);
			uvIndices.push_back(uv[0]);

			normalIndices.push_back(nv[0]);
			normalIndices.push_back(nv[1]);
			normalIndices.push_back(nv[2]);

			normalIndices.push_back(nv[2]);
			normalIndices.push_back(nv[3]);
			normalIndices.push_back(nv[0]);
		}

	}
	//the mesh data is finally calculated here
	for (unsigned int i = 0; i < vertexIndices.size(); i++)
	{
		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		// Get the attributes thanks to the index
		glm::vec3 vertex = vertices[vertexIndex - 1];
		glm::vec2 uv = uvs[uvIndex - 1];
		glm::vec3 normal = normals[normalIndex - 1];

		// Put the attributes in buffers
		out_vertices.push_back(vertex);
		out_uvs.push_back(uv);
		out_normals.push_back(normal);
	}

	return true;
}

void computeTangentBasis(std::vector<glm::vec3>& vertices, std::vector<glm::vec2>& uvs, std::vector<glm::vec3>& normals, std::vector<glm::vec3>& tangents, std::vector<glm::vec3>& bitangents)
{
	for (unsigned int i = 0; i < vertices.size(); i += 3) {

		// Shortcuts for vertices
		glm::vec3& v0 = vertices[i + 0];
		glm::vec3& v1 = vertices[i + 1];
		glm::vec3& v2 = vertices[i + 2];

		// Shortcuts for UVs
		glm::vec2& uv0 = uvs[i + 0];
		glm::vec2& uv1 = uvs[i + 1];
		glm::vec2& uv2 = uvs[i + 2];

		// Edges of the triangle : postion delta
		glm::vec3 deltaPos1 = v1 - v0;
		glm::vec3 deltaPos2 = v2 - v0;

		// UV delta
		glm::vec2 deltaUV1 = uv1 - uv0;
		glm::vec2 deltaUV2 = uv2 - uv0;

		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
		glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

		// Set the same tangent for all three vertices of the triangle.
		// They will be merged later, in vboindexer.cpp
		tangents.push_back(tangent);
		tangents.push_back(tangent);
		tangents.push_back(tangent);

		// Same thing for binormals
		bitangents.push_back(bitangent);
		bitangents.push_back(bitangent);
		bitangents.push_back(bitangent);

	}

	// See "Going Further"
	for (unsigned int i = 0; i < vertices.size(); i += 1)
	{
		glm::vec3& n = normals[i];
		glm::vec3& t = tangents[i];
		glm::vec3& b = bitangents[i];

		// Gram-Schmidt orthogonalize
		t = glm::normalize(t - n * glm::dot(n, t));

		// Calculate handedness
		if (glm::dot(glm::cross(n, t), b) < 0.0f) {
			t = t * -1.0f;
		}
	}
}


// Returns true iif v1 can be considered equal to v2
bool is_near(float v1, float v2) {
	return fabs(v1 - v2) < 0.01f;
}

// Searches through all already-exported vertices
// for a similar one.
// Similar = same position + same UVs + same normal
bool getSimilarVertexIndex(
	glm::vec3& in_vertex,
	glm::vec2& in_uv,
	glm::vec3& in_normal,
	std::vector<glm::vec3>& out_vertices,
	std::vector<glm::vec2>& out_uvs,
	std::vector<glm::vec3>& out_normals,
	unsigned short& result
) {

	// Lame linear search
	for (unsigned int i = 0; i < out_vertices.size(); i++) {
		if (
			is_near(in_vertex.x, out_vertices[i].x) &&
			is_near(in_vertex.y, out_vertices[i].y) &&
			is_near(in_vertex.z, out_vertices[i].z) &&
			is_near(in_uv.x, out_uvs[i].x) &&
			is_near(in_uv.y, out_uvs[i].y) &&
			is_near(in_normal.x, out_normals[i].x) &&
			is_near(in_normal.y, out_normals[i].y) &&
			is_near(in_normal.z, out_normals[i].z)
			) {
			result = i;
			return true;
		}
	}
	// No other vertex could be used instead.
	// Looks like we'll have to add it to the VBO.
	return false;
}

void indexVBO_TBN(
	std::vector<glm::vec3>& in_vertices,
	std::vector<glm::vec2>& in_uvs,
	std::vector<glm::vec3>& in_normals,
	std::vector<glm::vec3>& in_tangents,
	std::vector<glm::vec3>& in_bitangents,

	std::vector<unsigned short>& out_indices,
	std::vector<glm::vec3>& out_vertices,
	std::vector<glm::vec2>& out_uvs,
	std::vector<glm::vec3>& out_normals,
	std::vector<glm::vec3>& out_tangents,
	std::vector<glm::vec3>& out_bitangents
) {
	// For each input vertex
	for (unsigned int i = 0; i < in_vertices.size(); i++) {

		// Try to find a similar vertex in out_XXXX
		unsigned short index;
		bool found = getSimilarVertexIndex(in_vertices[i], in_uvs[i], in_normals[i], out_vertices, out_uvs, out_normals, index);

		if (found) { // A similar vertex is already in the VBO, use it instead !
			out_indices.push_back(index);

			// Average the tangents and the bitangents
			out_tangents[index] += in_tangents[i];
			out_bitangents[index] += in_bitangents[i];
		}
		else { // If not, it needs to be added in the output data.
			out_vertices.push_back(in_vertices[i]);
			out_uvs.push_back(in_uvs[i]);
			out_normals.push_back(in_normals[i]);
			out_tangents.push_back(in_tangents[i]);
			out_bitangents.push_back(in_bitangents[i]);
			out_indices.push_back((unsigned short)out_vertices.size() - 1);
		}
	}
}