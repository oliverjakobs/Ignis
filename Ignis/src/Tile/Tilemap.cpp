#include "Tilemap.h"

#include "Obelisk/Debugger.h"


TileMap::TileMap(const std::vector<int>& tiles, int width, int height, float tileSize, const std::map<unsigned int, TileType>& typeMap)
	: m_width(width), m_height(height), m_tileSize(tileSize), m_gravity(glm::vec2())
{
	// nested loop iterating over the width and height of the map
	for (unsigned int i = 0; i < m_height; i++)
	{
		for (unsigned int j = 0; j < m_width; j++)
		{
			Tile tile;
			tile.Position = glm::vec2(j, m_height - (i + 1)) * m_tileSize;
			tile.ID = tiles.at(i * (unsigned int)m_width + j);

			// Get the type for the id;
			if (typeMap.find(tile.ID) != typeMap.end())
				tile.Type = typeMap.at(tile.ID);
			else
				tile.Type = TileType::TILE_EMPTY;

			m_tiles.push_back(tile);
		}
	}
}

TileMap::~TileMap()
{

}

void TileMap::SetGravity(const glm::vec2& gravity)
{
	m_gravity = gravity;
}

std::shared_ptr<Body> TileMap::CreateBody(float x, float y, float hWidth, float hHeight, BodyType type)
{
	return std::shared_ptr<Body>();

	/*auto body = std::make_shared<Body>(this, glm::vec2(x, y), glm::vec2(hWidth, hHeight), type);

	m_bodies.push_back(body);

	return body;*/
}

std::shared_ptr<Body> TileMap::CreateBody(const glm::vec2& pos, const glm::vec2& halfDim, BodyType type)
{
	return std::shared_ptr<Body>();

	/*auto body = std::make_shared<Body>(this, pos, halfDim, type);

	m_bodies.push_back(body);

	return body;*/
}

void TileMap::DestroyBody(const std::shared_ptr<Body>& body)
{
	auto it = std::find_if(m_bodies.begin(), m_bodies.end(), [&](auto& e) { return e == body; });

	if (it != m_bodies.end())
	{
		m_bodies.erase(it);
	}
}

void TileMap::OnRender() const
{
	//m_renderer->RenderMap(glm::mat4(), View::GetMat(), glm::mat4());
}

void TileMap::OnRenderDebug() const
{
	/*for (auto& tile : m_tiles)
	{
		if (tile.type == TILE_SOLID)
			Primitives::DrawRect(tile.position.x, tile.position.y, m_tileSize, m_tileSize, RED, Renderer::GetViewMat());
		else if (tile.type == TILE_PLATFORM)
			Primitives::DrawRect(tile.position.x, tile.position.y, m_tileSize, m_tileSize, BLUE, Renderer::GetViewMat());
		else if (tile.type == TILE_SLOPE_LEFT)
			Primitives::DrawPolygon({ tile.position, tile.position + glm::vec2(m_tileSize, 0.0f),  tile.position + glm::vec2(0.0f, m_tileSize) }, MAGENTA, Renderer::GetViewMat());
		else if (tile.type == TILE_SLOPE_RIGHT)
			Primitives::DrawPolygon({ tile.position, tile.position + glm::vec2(m_tileSize, 0.0f),  tile.position + glm::vec2(m_tileSize) }, MAGENTA, Renderer::GetViewMat());
	}*/
}

int TileMap::GetWidth() const
{
	return m_width;
}

int TileMap::GetHeight() const
{
	return m_height;
}

float TileMap::GetTileSize() const
{
	return m_tileSize;
}

glm::vec2 TileMap::GetDimension() const
{
	return glm::vec2(m_width, m_height);
}

Rect TileMap::GetConstraint() const
{
	return  Rect(glm::vec2(), GetDimension() * m_tileSize);
}

unsigned int TileMap::GetIndexF(float x, float y) const
{
	return GetIndex(GetTilePos(glm::vec2(x, y)));
}

unsigned int TileMap::GetIndexF(const glm::vec2& pos) const
{
	return GetIndex(GetTilePos(pos));
}

unsigned int TileMap::GetIndex(unsigned int x, unsigned int y) const
{
	return (m_height - y - 1) * m_width + x;
}

unsigned int TileMap::GetIndex(const glm::ivec2& pos) const
{
	return GetIndex(pos.x, pos.y);
}

Tile* TileMap::at(unsigned int index)
{
	if (index >= m_tiles.size())
		return nullptr;

	return &m_tiles.at(index);
}

const Tile* TileMap::at(unsigned int index) const
{
	return &m_tiles.at(index);
}

Tile* TileMap::operator[](unsigned int index)
{
	return &m_tiles[index];
}

const Tile* TileMap::operator[](unsigned int index) const
{
	return &m_tiles[index];
}

glm::ivec2 TileMap::GetTilePos(float x, float y) const
{
	return glm::ivec2(static_cast<int>(std::floor(x / m_tileSize)), static_cast<int>(std::floor(y / m_tileSize)));
}

glm::ivec2 TileMap::GetTilePos(const glm::vec2& pos) const
{
	return GetTilePos(pos.x, pos.y);
}

std::vector<const Tile*> TileMap::GetAdjacentTiles(float x, float y, float w, float h)
{
	std::vector<const Tile*> tiles;

	glm::ivec2 start = GetTilePos(x, y);
	glm::ivec2 end = GetTilePos(x + w, y + h);

	for (int i = start.x; i <= end.x; i++)
	{
		for (int j = start.y; j <= end.y; j++)
		{
			if (at(GetIndex(i, j)) != nullptr)
				tiles.push_back(at(GetIndex(i, j)));
		}
	}

	return tiles;
}

std::vector<const Tile*> TileMap::GetAdjacentTiles(const glm::vec2& pos, const glm::vec2& size)
{
	return GetAdjacentTiles(pos.x, pos.y, size.x, size.y);
}

glm::vec2 TileMap::GetGravity() const
{
	return m_gravity;
}

std::vector<Tile> TileMap::GetTiles() const
{
	return m_tiles;
}

std::vector<Line> TileMap::ToEdges() const
{
	struct EdgeData
	{
		int edgeID[4];
		bool edgeExist[4];
	};

	unsigned int northIndex = 0;
	unsigned int southIndex = 1;
	unsigned int eastIndex = 2;
	unsigned int westIndex = 3;

	std::vector<Line> edges;
	edges.push_back(Line(0.0f, 0.0f, m_width * m_tileSize, 0.0f));
	edges.push_back(Line(m_width * m_tileSize, 0.0f, m_width * m_tileSize, m_height * m_tileSize));
	edges.push_back(Line(m_width * m_tileSize, m_height * m_tileSize, 0.0f, m_height * m_tileSize));
	edges.push_back(Line(0.0f, m_height * m_tileSize, 0.0f, 0.0f));

	std::vector<EdgeData> edgeData(m_tiles.size());

	// iterate through all tiles from top left to bottom right
	for (int x = 1; x < m_width - 1; x++)
		for (int y = 1; y < m_height - 1; y++)
		{
			// some convenient indices
			int i = y * m_width + x;		// this
			int n = (y - 1) * m_width + x;	// northern neighbour
			int s = (y + 1) * m_width + x;	// southern neighbour
			int w = y * m_width + (x - 1);	// western neighbour
			int e = y * m_width + (x + 1);	// eastern neighbour

			// if this tile is solid, check if it needs edges
			if (m_tiles[i].Type == TileType::TILE_SOLID)
			{
				// if there is no western neighbour, it needs a western edge
				if (m_tiles[w].Type != TileType::TILE_SOLID)
				{
					// either extend it from its northern neighbour (if they have one) or start a new one
					if (edgeData[n].edgeExist[westIndex])
					{
						// northern neighbour has a western edge, so extend it downwards
						edges[edgeData[n].edgeID[westIndex]].end.y += m_tileSize;
						edgeData[i].edgeID[westIndex] = edgeData[n].edgeID[westIndex];
						edgeData[i].edgeExist[westIndex] = true;
					}
					else
					{
						// northern neighbour does not have a western edge, so create one and add it to the polygon pool
						int edgeID = edges.size();
						edges.push_back(Line(x * m_tileSize, y * m_tileSize, x * m_tileSize, y * m_tileSize + m_tileSize));

						// update tile information with edge information
						edgeData[i].edgeID[westIndex] = edgeID;
						edgeData[i].edgeExist[westIndex] = true;
					}
				}

				// if there is no eastern neighbour, it needs a eastern edge
				if (m_tiles[e].Type != TileType::TILE_SOLID)
				{
					// either extend it from its northern neighbour (if they have one) or start a new one
					if (edgeData[n].edgeExist[eastIndex])
					{
						// northern neighbour has a eastern edge, so extend it downwards
						edges[edgeData[n].edgeID[eastIndex]].end.y += m_tileSize;
						edgeData[i].edgeID[eastIndex] = edgeData[n].edgeID[eastIndex];
						edgeData[i].edgeExist[eastIndex] = true;
					}
					else
					{
						// northern neighbour does not have a eastern edge, so create one and add it to the polygon pool
						int edgeID = edges.size();
						edges.push_back(Line((x + 1) * m_tileSize, y * m_tileSize, (x + 1) * m_tileSize, y * m_tileSize + m_tileSize));

						// update tile information with edge information
						edgeData[i].edgeID[eastIndex] = edgeID;
						edgeData[i].edgeExist[eastIndex] = true;
					}
				}

				// if there is no northern neighbour, it needs a northern edge
				if (m_tiles[n].Type != TileType::TILE_SOLID)
				{
					// either extend it from its western neighbour (if they have one) or start a new one
					if (edgeData[w].edgeExist[northIndex])
					{
						// western neighbour has a northern edge, so extend it eastwards
						edges[edgeData[w].edgeID[northIndex]].end.x += m_tileSize;
						edgeData[i].edgeID[northIndex] = edgeData[w].edgeID[northIndex];
						edgeData[i].edgeExist[northIndex] = true;
					}
					else
					{
						// western neighbour does not have a northern edge, so create one and add it to the polygon pool
						int edgeID = edges.size();
						edges.push_back(Line(x * m_tileSize, y * m_tileSize, x * m_tileSize + m_tileSize, y * m_tileSize));

						// update tile information with edge information
						edgeData[i].edgeID[northIndex] = edgeID;
						edgeData[i].edgeExist[northIndex] = true;
					}
				}

				// if there is no southern neighbour, it needs a southern edge
				if (m_tiles[s].Type != TileType::TILE_SOLID)
				{
					// either extend it from its western neighbour (if they have one) or start a new one
					if (edgeData[w].edgeExist[southIndex])
					{
						// western neighbour has a southern edge, so extend it eastwards
						edges[edgeData[w].edgeID[southIndex]].end.x += m_tileSize;
						edgeData[i].edgeID[southIndex] = edgeData[w].edgeID[southIndex];
						edgeData[i].edgeExist[southIndex] = true;
					}
					else
					{
						// western neighbour does not have a southern edge, so create one and add it to the polygon pool
						int edgeID = edges.size();
						edges.push_back(Line(x * m_tileSize, (y + 1) * m_tileSize, x * m_tileSize + m_tileSize, (y + 1) * m_tileSize));

						// update tile information with edge information
						edgeData[i].edgeID[southIndex] = edgeID;
						edgeData[i].edgeExist[southIndex] = true;
					}
				}
			}
		}
	return edges;
}

std::vector<std::shared_ptr<Body>> const& TileMap::GetBodies() const
{
	return m_bodies;
}

std::vector<Body*> TileMap::GetBodiesT(BodyType type) const
{
	std::vector<Body*> typed;

	for (auto& b : m_bodies)
	{
		if (b->GetType() == type)
			typed.push_back(b.get());
	}

	return typed;
}

std::vector<Body*> TileMap::GetOtherBodies(Body* body) const
{
	std::vector<Body*> typed;

	for (auto& b : m_bodies)
	{
		if (b.get() != body)
			typed.push_back(b.get());
	}

	return typed;
}
