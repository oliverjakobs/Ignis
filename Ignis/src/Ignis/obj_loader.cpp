#include "obj_loader.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <map>

#include "Core/Debugger.h"

uint FindNextChar(uint start, const char* str, uint length, char token)
{
	uint result = start;
	while (result < length)
	{
		result++;
		if (str[result] == token)
			break;
	}

	return result;
}

uint ParseOBJIndexValue(const std::string& token, uint start, uint end)
{
	return atoi(token.substr(start, end - start).c_str()) - 1;
}

OBJIndex ParseOBJIndex(const std::string& token, bool* hasUVs, bool* hasNormals)
{
	unsigned int tokenLength = token.length();
	const char* tokenString = token.c_str();

	unsigned int vertIndexStart = 0;
	unsigned int vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, '/');

	OBJIndex result;
	result.Vertex = ParseOBJIndexValue(token, vertIndexStart, vertIndexEnd);
	result.UV = 0;
	result.Normal = 0;

	if (vertIndexEnd >= tokenLength)
		return result;

	vertIndexStart = vertIndexEnd + 1;
	vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, '/');

	result.UV = ParseOBJIndexValue(token, vertIndexStart, vertIndexEnd);
	*hasUVs = true;

	if (vertIndexEnd >= tokenLength)
		return result;

	vertIndexStart = vertIndexEnd + 1;
	vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, '/');

	result.Normal = ParseOBJIndexValue(token, vertIndexStart, vertIndexEnd);
	*hasNormals = true;

	return result;
}

float ParseOBJFloatValue(const std::string& token, unsigned int start, unsigned int end)
{
	return (float)atof(token.substr(start, end - start).c_str());
}

glm::vec2 ParseOBJVec2(const std::string& line)
{
	unsigned int tokenLength = line.length();
	const char* tokenString = line.c_str();

	unsigned int vertIndexStart = 3;

	while (vertIndexStart < tokenLength)
	{
		if (tokenString[vertIndexStart] != ' ')
			break;
		vertIndexStart++;
	}

	unsigned int vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');

	float x = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);

	vertIndexStart = vertIndexEnd + 1;
	vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');

	float y = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);

	return glm::vec2(x, y);
}

glm::vec3 ParseOBJVec3(const std::string& line)
{
	unsigned int tokenLength = line.length();
	const char* tokenString = line.c_str();

	unsigned int vertIndexStart = 2;

	while (vertIndexStart < tokenLength)
	{
		if (tokenString[vertIndexStart] != ' ')
			break;
		vertIndexStart++;
	}

	unsigned int vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');

	float x = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);

	vertIndexStart = vertIndexEnd + 1;
	vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');

	float y = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);

	vertIndexStart = vertIndexEnd + 1;
	vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');

	float z = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);

	return glm::vec3(x, y, z);
}

std::vector<std::string> SplitString(const std::string& s, char delim)
{
	std::vector<std::string> elems;

	const char* cstr = s.c_str();
	unsigned int strLength = s.length();
	unsigned int start = 0;
	unsigned int end = 0;

	while (end <= strLength)
	{
		while (end <= strLength)
		{
			if (cstr[end] == delim)
				break;
			end++;
		}

		elems.push_back(s.substr(start, end - start));
		start = end + 1;
		end = start;
	}

	return elems;
}

OBJModel::OBJModel(const std::string& filename)
{
	HasUVs = false;
	HasNormals = false;
	std::ifstream file;
	file.open(filename.c_str());

	std::string line;
	if(file.is_open())
	{
		while(file.good())
		{
			getline(file, line);
		
			unsigned int lineLength = line.length();
			
			if(lineLength < 2)
				continue;
			
			const char* lineCStr = line.c_str();
			
			switch(lineCStr[0])
			{
				case 'v':
					if(lineCStr[1] == 't')
						this->UVs.push_back(ParseOBJVec2(line));
					else if(lineCStr[1] == 'n')
						this->Normals.push_back(ParseOBJVec3(line));
					else if(lineCStr[1] == ' ' || lineCStr[1] == '\t')
						this->Vertices.push_back(ParseOBJVec3(line));
					break;
				case 'f':
					CreateOBJFace(line);
					break;
				default: break;
			};
		}
	}
	else
	{
		DEBUG_ERROR("Unable to load mesh: {0}", filename);
	}
}

void IndexedModel::CalcNormals()
{
	for(unsigned int i = 0; i < Indices.size(); i += 3)
	{
		int i0 = Indices[i];
		int i1 = Indices[i + 1];
		int i2 = Indices[i + 2];

		glm::vec3 v1 = Positions[i1] - Positions[i0];
		glm::vec3 v2 = Positions[i2] - Positions[i0];
		
		glm::vec3 normal = glm::normalize(glm::cross(v1, v2));
			
		Normals[i0] += normal;
		Normals[i1] += normal;
		Normals[i2] += normal;
	}
	
	for(unsigned int i = 0; i < Positions.size(); i++)
		Normals[i] = glm::normalize(Normals[i]);
}

IndexedModel OBJModel::ToIndexedModel()
{
	IndexedModel result;
	IndexedModel normalModel;
	
	unsigned int numIndices = OBJIndices.size();
	
	std::vector<OBJIndex*> indexLookup;
	
	for(unsigned int i = 0; i < numIndices; i++)
		indexLookup.push_back(&OBJIndices[i]);
	
	std::sort(indexLookup.begin(), indexLookup.end());
	
	std::map<OBJIndex, unsigned int> normalModelIndexMap;
	std::map<unsigned int, unsigned int> indexMap;
	
	for(unsigned int i = 0; i < numIndices; i++)
	{
		OBJIndex* currentIndex = &OBJIndices[i];
		
		glm::vec3 currentPosition = Vertices[currentIndex->Vertex];
		glm::vec2 currentTexCoord;
		glm::vec3 currentNormal;
		
		if(HasUVs)
			currentTexCoord = UVs[currentIndex->UV];
		else
			currentTexCoord = glm::vec2(0,0);
			
		if(HasNormals)
			currentNormal = Normals[currentIndex->Normal];
		else
			currentNormal = glm::vec3(0,0,0);
		
		unsigned int normalModelIndex;
		unsigned int resultModelIndex;
		
		//Create model to properly generate normals on
		std::map<OBJIndex, unsigned int>::iterator it = normalModelIndexMap.find(*currentIndex);
		if(it == normalModelIndexMap.end())
		{
			normalModelIndex = normalModel.Positions.size();
		
			normalModelIndexMap.insert(std::pair<OBJIndex, unsigned int>(*currentIndex, normalModelIndex));
			normalModel.Positions.push_back(currentPosition);
			normalModel.TexCoords.push_back(currentTexCoord);
			normalModel.Normals.push_back(currentNormal);
		}
		else
			normalModelIndex = it->second;
		
		//Create model which properly separates texture coordinates
		unsigned int previousVertexLocation = FindLastVertexIndex(indexLookup, currentIndex, result);
		
		if(previousVertexLocation == (unsigned int)-1)
		{
			resultModelIndex = result.Positions.size();
		
			result.Positions.push_back(currentPosition);
			result.TexCoords.push_back(currentTexCoord);
			result.Normals.push_back(currentNormal);
		}
		else
			resultModelIndex = previousVertexLocation;
		
		normalModel.Indices.push_back(normalModelIndex);
		result.Indices.push_back(resultModelIndex);
		indexMap.insert(std::pair<unsigned int, unsigned int>(resultModelIndex, normalModelIndex));
	}
	
	if(!HasNormals)
	{
		normalModel.CalcNormals();
		
		for(unsigned int i = 0; i < result.Positions.size(); i++)
			result.Normals[i] = normalModel.Normals[indexMap[i]];
	}
	
	return result;
};

unsigned int OBJModel::FindLastVertexIndex(const std::vector<OBJIndex*>& indexLookup, const OBJIndex* currentIndex, const IndexedModel& result)
{
	unsigned int start = 0;
	unsigned int end = indexLookup.size();
	unsigned int current = (end - start) / 2 + start;
	unsigned int previous = start;
	
	while(current != previous)
	{
		OBJIndex* testIndex = indexLookup[current];
		
		if(testIndex->Vertex == currentIndex->Vertex)
		{
			unsigned int countStart = current;
		
			for(unsigned int i = 0; i < current; i++)
			{
				OBJIndex* possibleIndex = indexLookup[current - i];
				
				if(possibleIndex == currentIndex)
					continue;
					
				if(possibleIndex->Vertex != currentIndex->Vertex)
					break;
					
				countStart--;
			}
			
			for(unsigned int i = countStart; i < indexLookup.size() - countStart; i++)
			{
				OBJIndex* possibleIndex = indexLookup[current + i];
				
				if(possibleIndex == currentIndex)
					continue;
					
				if(possibleIndex->Vertex != currentIndex->Vertex)
					break;
				else if((!HasUVs || possibleIndex->UV == currentIndex->UV) 
					&& (!HasNormals || possibleIndex->Normal == currentIndex->Normal))
				{
					glm::vec3 currentPosition = Vertices[currentIndex->Vertex];
					glm::vec2 currentTexCoord;
					glm::vec3 currentNormal;
					
					if(HasUVs)
						currentTexCoord = UVs[currentIndex->UV];
					else
						currentTexCoord = glm::vec2(0,0);
						
					if(HasNormals)
						currentNormal = Normals[currentIndex->Normal];
					else
						currentNormal = glm::vec3(0,0,0);
					
					for(unsigned int j = 0; j < result.Positions.size(); j++)
					{
						if(currentPosition == result.Positions[j] 
							&& ((!HasUVs || currentTexCoord == result.TexCoords[j])
							&& (!HasNormals || currentNormal == result.Normals[j])))
						{
							return j;
						}
					}
				}
			}
		
			return -1;
		}
		else
		{
			if(testIndex->Vertex < currentIndex->Vertex)
				start = current;
			else
				end = current;
		}
	
		previous = current;
		current = (end - start) / 2 + start;
	}
	
	return -1;
}

void OBJModel::CreateOBJFace(const std::string& line)
{
	std::vector<std::string> tokens = SplitString(line, ' ');

	OBJIndices.push_back(ParseOBJIndex(tokens[1], &HasUVs, &HasNormals));
	OBJIndices.push_back(ParseOBJIndex(tokens[2], &HasUVs, &HasNormals));
	OBJIndices.push_back(ParseOBJIndex(tokens[3], &HasUVs, &HasNormals));

	if((int)tokens.size() > 4)
	{
		OBJIndices.push_back(ParseOBJIndex(tokens[1], &HasUVs, &HasNormals));
		OBJIndices.push_back(ParseOBJIndex(tokens[3], &HasUVs, &HasNormals));
		OBJIndices.push_back(ParseOBJIndex(tokens[4], &HasUVs, &HasNormals));
	}
}