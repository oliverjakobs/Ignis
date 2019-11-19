#include "Algorithm.h"

#include "Obelisk/Debugger.h"

namespace tile
{
	std::vector<glm::vec3> Visibility(const glm::vec2& pos, std::vector<Line> edges)
	{
		std::vector<glm::vec3> intersections;

		for (auto& e : edges)
		{
			for (size_t i = 0; i < 2; i++)
			{
				glm::vec2  v;

				if (i == 0)	v = e.Start;
				if (i == 1)	v = e.End;

				glm::vec2 rd = v - pos;

				float base_ang = atan2f(rd.y, rd.x);

				float ang = 0;

				// For each point, cast 3 rays, 1 directly at point and 1 a little bit either side
				for (int j = 0; j < 3; j++)
				{
					if (j == 0)	ang = base_ang - 0.0001f;
					if (j == 1)	ang = base_ang;
					if (j == 2)	ang = base_ang + 0.0001f;

					// Create ray along angle for required distance
					rd = glm::vec2(cosf(ang), sinf(ang));

					float min_t1 = INFINITY;
					glm::vec2 minPos = glm::vec2();
					float minAng = 0;
					bool valid = false;

					// Check for ray intersection with all edges
					for (auto& e : edges)
					{
						// Create line segment vector
						glm::vec2 sd = e.End - e.Start;

						if (fabs(sd.x - rd.x) > 0.0f && fabs(sd.y - rd.y) > 0.0f)
						{
							// t2 is normalised distance from line segment start to line segment end of intersect point
							float t2 = (rd.x * (e.Start.y - pos.y) + (rd.y * (pos.x - e.Start.x))) / (sd.x * rd.y - sd.y * rd.x);
							// t1 is normalised distance from source along ray to ray length of intersect point
							float t1 = (e.Start.x + sd.x * t2 - pos.x) / rd.x;

							// If intersect point exists along ray, and along line 
							// segment then intersect point is valid
							if (t1 > 0 && t2 >= 0 && t2 <= 1.0f)
							{
								// Check if this intersect point is closest to source. If
								// it is, then store this point and reject others
								if (t1 < min_t1)
								{
									min_t1 = t1;
									minPos = pos + rd * t1;
									minAng = glm::atan(minPos.y - pos.y, minPos.x - pos.x);
									valid = true;
								}
							}
						}
					}

					if (valid)// Add intersection point to visibility polygon perimeter
						intersections.push_back({ minPos, minAng });
				}
			}
		}

		std::sort(intersections.begin(), intersections.end(), [&](const auto& t1, const auto& t2)
		{
			return t1.z < t2.z;
		});

		return intersections;
	}

	TileType GetTileType(const std::vector<Tile>& tiles, int32_t index)
	{
		if (index < 0 || index >= tiles.size())
			return TileType::TILE_EMPTY;

		return tiles[index].Type;
	}

	std::vector<Line> GetEdges(const TileMap& map)
	{
		std::vector<Line> edges;

		for (auto& chunk : map.GetChunks())
		{
			auto e = GetEdges(chunk.Tiles, map.GetChunkSize(), map.GetChunkSize(), map.GetTileSize(), map.GetChunkOffset(chunk));
			edges.insert(edges.end(), e.begin(), e.end());
		}

		edges.push_back(Line(0.0f, 0.0f, map.GetWidth() * map.GetTileSize(), 0.0f));
		edges.push_back(Line(map.GetWidth() * map.GetTileSize(), 0.0f, map.GetWidth() * map.GetTileSize(), map.GetHeight() * map.GetTileSize()));
		edges.push_back(Line(map.GetWidth() * map.GetTileSize(), map.GetHeight() * map.GetTileSize(), 0.0f, map.GetHeight() * map.GetTileSize()));
		edges.push_back(Line(0.0f, map.GetHeight() * map.GetTileSize(), 0.0f, 0.0f));

		return edges;
	}

	std::vector<Line> GetEdges(const std::vector<Tile>& tiles, size_t width, size_t height, float tileSize, const glm::vec2& offset)
	{
		struct EdgeData
		{
			size_t edgeID[4];
			bool edgeExist[4];
		};

		unsigned int northIndex = 0;
		unsigned int southIndex = 1;
		unsigned int eastIndex = 2;
		unsigned int westIndex = 3;

		// TODO: fist and last tiles detection and border outside of this function
		std::vector<Line> edges;
		std::vector<EdgeData> edgeData(tiles.size());

		// iterate through all tiles from bottom left to top right
		for (size_t x = 0; x < width; x++)
			for (size_t y = 0; y < height; y++)
			{
				// some convenient indices
				int32_t i = y * width + x;			// this
				int32_t n = (y + 1) * width + x;	// northern neighbour
				int32_t s = (y - 1) * width + x;	// southern neighbour
				int32_t w = y * width + (x - 1);	// western neighbour
				int32_t e = y * width + (x + 1);	// eastern neighbour

				// if this tile is solid, check if it needs edges
				if (GetTileType(tiles, i) == TileType::TILE_SOLID)
				{
					// if there is no western neighbour, it needs a western edge
					if (GetTileType(tiles, w) != TileType::TILE_SOLID)
					{
						// either extend it from its southern neighbour (if they have one) or start a new one
						if (s > 0 && edgeData[s].edgeExist[westIndex])
						{
							// southern neighbour has a western edge, so extend it upwards
							edges[edgeData[s].edgeID[westIndex]].End.y += tileSize;
							edgeData[i].edgeID[westIndex] = edgeData[s].edgeID[westIndex];
							edgeData[i].edgeExist[westIndex] = true;
						}
						else
						{
							// southern neighbour does not have a western edge, so create one and add it to the polygon pool
							size_t edgeID = edges.size();
							edges.push_back(Line(x * tileSize, y * tileSize, x * tileSize, y * tileSize + tileSize) + offset);

							// update tile information with edge information
							edgeData[i].edgeID[westIndex] = edgeID;
							edgeData[i].edgeExist[westIndex] = true;
						}
					}

					// if there is no eastern neighbour, it needs a eastern edge
					if (GetTileType(tiles, e) != TileType::TILE_SOLID)
					{
						// either extend it from its southern neighbour (if they have one) or start a new one
						if (s > 0 && edgeData[s].edgeExist[eastIndex])
						{
							// southern neighbour has a eastern edge, so extend it downwards
							edges[edgeData[s].edgeID[eastIndex]].End.y += tileSize;
							edgeData[i].edgeID[eastIndex] = edgeData[s].edgeID[eastIndex];
							edgeData[i].edgeExist[eastIndex] = true;
						}
						else
						{
							// southern neighbour does not have a eastern edge, so create one and add it to the polygon pool
							size_t edgeID = edges.size();
							edges.push_back(Line((x + 1) * tileSize, y * tileSize, (x + 1) * tileSize, y * tileSize + tileSize) + offset);

							// update tile information with edge information
							edgeData[i].edgeID[eastIndex] = edgeID;
							edgeData[i].edgeExist[eastIndex] = true;
						}
					}

					// if there is no northern neighbour, it needs a northern edge
					if (GetTileType(tiles, n) != TileType::TILE_SOLID)
					{
						// either extend it from its western neighbour (if they have one) or start a new one
						if (w > 0 && edgeData[w].edgeExist[northIndex])
						{
							// western neighbour has a northern edge, so extend it eastwards
							edges[edgeData[w].edgeID[northIndex]].End.x += tileSize;
							edgeData[i].edgeID[northIndex] = edgeData[w].edgeID[northIndex];
							edgeData[i].edgeExist[northIndex] = true;
						}
						else
						{
							// western neighbour does not have a northern edge, so create one and add it to the polygon pool
							size_t edgeID = edges.size();
							edges.push_back(Line(x * tileSize, (y + 1) * tileSize, x * tileSize + tileSize, (y + 1) * tileSize) + offset);

							// update tile information with edge information
							edgeData[i].edgeID[northIndex] = edgeID;
							edgeData[i].edgeExist[northIndex] = true;
						}
					}

					// if there is no southern neighbour, it needs a southern edge
					if (GetTileType(tiles, s) != TileType::TILE_SOLID)
					{
						// either extend it from its western neighbour (if they have one) or start a new one
						if (w > 0 && edgeData[w].edgeExist[southIndex])
						{
							// western neighbour has a southern edge, so extend it eastwards
							edges[edgeData[w].edgeID[southIndex]].End.x += tileSize;
							edgeData[i].edgeID[southIndex] = edgeData[w].edgeID[southIndex];
							edgeData[i].edgeExist[southIndex] = true;
						}
						else
						{
							// western neighbour does not have a southern edge, so create one and add it to the polygon pool
							size_t edgeID = edges.size();
							edges.push_back(Line(x * tileSize, y * tileSize, x * tileSize + tileSize, y * tileSize) + offset);

							// update tile information with edge information
							edgeData[i].edgeID[southIndex] = edgeID;
							edgeData[i].edgeExist[southIndex] = true;
						}
					}
				}
			}
		return edges;
	}
}