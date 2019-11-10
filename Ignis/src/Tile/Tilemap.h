#pragma once

#include "Body.h"

#include "TileRenderer.h"

class TileMap
{
private:
	// map settings
	int m_width;
	int m_height;

	float m_tileSize;

	glm::vec2 m_gravity;

	std::vector<Tile> m_tiles;

	std::vector<std::shared_ptr<Body>> m_bodies;

public:
	TileMap(const std::vector<int>& tiles, int width, int height, float tileSize, const std::map<unsigned int, TileType>& typeMap);
	~TileMap();

	void SetGravity(const glm::vec2& gravity);

	// only way to create bodies
	std::shared_ptr<Body> CreateBody(float x, float y, float hWidth, float hHeight, BodyType type);
	std::shared_ptr<Body> CreateBody(const glm::vec2& pos, const glm::vec2& halfDim, BodyType type);
	void DestroyBody(const std::shared_ptr<Body>& body);

	// loop functions
	void OnRender() const;
	void OnRenderDebug() const;

	int GetWidth() const;
	int GetHeight() const;
	float GetTileSize() const;
	glm::vec2 GetDimension() const;
	Rect GetConstraint() const;

	unsigned int GetIndexF(float x, float y) const;
	unsigned int GetIndexF(const glm::vec2& pos) const;
	unsigned int GetIndex(unsigned int x, unsigned int y) const;
	unsigned int GetIndex(const glm::ivec2& pos) const;

	Tile* at(unsigned int index);
	const Tile* at(unsigned int index) const;
	Tile* operator[](unsigned int index);
	const Tile* operator[](unsigned int index) const;

	// calculate map coords from world coords
	glm::ivec2 GetTilePos(float x, float y) const;
	glm::ivec2 GetTilePos(const glm::vec2& pos) const;

	// get all tiles in an area (in world coords)
	std::vector<const Tile*> GetAdjacentTiles(float x, float y, float w, float h);
	std::vector<const Tile*> GetAdjacentTiles(const glm::vec2& pos, const glm::vec2& size);
	
	glm::vec2 GetGravity() const;

	std::vector<Tile> GetTiles() const;
	std::vector<Line> ToEdges() const;

	// get bodies
	std::vector<std::shared_ptr<Body>> const& GetBodies() const;
	std::vector<Body*> GetBodiesT(BodyType type) const;

	std::vector<Body*> GetOtherBodies(Body* body) const;
};
