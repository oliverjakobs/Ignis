#ifndef OBJ_LOADER_H_INCLUDED
#define OBJ_LOADER_H_INCLUDED

#include <glm/glm.hpp>
#include <vector>
#include <string>

#include "Core/Types.h"

struct OBJIndex
{
    unsigned int Vertex;
    unsigned int UV;
    unsigned int Normal;
    
    bool operator<(const OBJIndex& r) const { return Vertex < r.Vertex; }
};

struct IndexedModel
{
    std::vector<glm::vec3> Positions;
    std::vector<glm::vec2> TexCoords;
    std::vector<glm::vec3> Normals;
    std::vector<unsigned int> Indices;
    
    void CalcNormals();
};

class OBJModel
{
public:
    std::vector<OBJIndex> OBJIndices;
    std::vector<glm::vec3> Vertices;
    std::vector<glm::vec2> UVs;
    std::vector<glm::vec3> Normals;
    bool HasUVs;
    bool HasNormals;
    
    OBJModel(const std::string& fileName);
    
    IndexedModel ToIndexedModel();
private:
    unsigned int FindLastVertexIndex(const std::vector<OBJIndex*>& indexLookup, const OBJIndex* currentIndex, const IndexedModel& result);
    void CreateOBJFace(const std::string& line);
};

#endif // OBJ_LOADER_H_INCLUDED
