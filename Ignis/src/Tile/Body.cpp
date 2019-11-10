#include "Body.h"

#include "Tilemap.h"

Body::Body(TileMap* map, const glm::vec2& pos, const glm::vec2& halfDim, BodyType type)
	: m_map(map), m_position(pos), m_halfDimension(halfDim), m_type(type)
{
	m_velocity = glm::vec2();
	m_sensorOffset = TILE_SENSOR_OFFSET;

	m_collidesBottom = false;
	m_collidesTop = false;
	m_collidesLeft = false;
	m_collidesRight = false;
	m_onSlope = false;
	m_slopeDetected = false;

	m_gravityScale = 1.0f;

	m_drop = false;
}

void Body::MoveX(float x)
{
	m_collidesLeft = false;
	m_collidesRight = false;

	glm::vec2 oldPosition = m_position;

	m_position.x += x;

	float wallX = 0.0f;
	if (m_velocity.x <= 0.0f && CheckLeft(m_position, oldPosition, &wallX))
	{
		m_position.x = wallX + m_halfDimension.x;
		m_velocity.x = 0.0f;
		m_collidesLeft = true;
	}

	wallX = 0.0f;
	if (m_velocity.x >= 0.0f && CheckRight(m_position, oldPosition, &wallX))
	{
		m_position.x = wallX - m_halfDimension.x;
		m_velocity.x = 0.0f;
		m_collidesRight = true;
	}
}

void Body::MoveY(float y)
{
	m_collidesBottom = false;
	m_collidesTop = false;

	glm::vec2 oldPosition = m_position;
	m_position.y += y;

	m_onSlope = false;

	float groundY = 0.0f;
	if (m_velocity.y <= 0.0f && CheckBottom(m_position, oldPosition, &groundY))
	{
		m_position.y = groundY + m_halfDimension.y;
		m_velocity.y = 0.0f;
		m_collidesBottom = true;
	}

	groundY = 0.0f;
	if (m_velocity.y >= 0.0f && CheckTop(m_position, oldPosition, &groundY))
	{
		m_position.y = groundY - m_halfDimension.y;
		m_velocity.y = 0.0f;
		m_collidesTop = true;
	}
}

bool Body::CheckBottom(const glm::vec2& position, const glm::vec2& oldPosition, float* groundY)
{
	Line sensor = GetSensorBottom(position, m_offsetVertical);
	Line oldSensor = GetSensorBottom(oldPosition, m_offsetVertical);

	auto tiles = m_map->GetAdjacentTiles(sensor.start, oldSensor.end - sensor.start + (m_slopeDetected ? glm::vec2(0.0f, m_map->GetTileSize() / 2.0f) : glm::vec2()));

	std::sort(tiles.begin(), tiles.end(), [](const Tile* t1, const Tile* t2)
	{
		if (t1->Position.y == t2->Position.y)
			return t1->Type < t2->Type;

		return t1->Position.y > t2->Position.y;
	});

	for (auto& t : tiles)
	{
		if (t->Type == TileType::TILE_SOLID && !m_onSlope)
		{
			*groundY = t->Position.y + m_map->GetTileSize();
			return true;
		}
		else if (t->Type == TileType::TILE_SLOPE_LEFT)
		{
			float x = m_position.x - m_halfDimension.x - t->Position.x;

			*groundY = t->Position.y + m_map->GetTileSize() - x;
			m_onSlope = true;
			return true;
		}
		else if (t->Type == TileType::TILE_SLOPE_RIGHT)
		{
			float x = m_position.x + m_halfDimension.x - t->Position.x;

			*groundY = t->Position.y + x;
			m_onSlope = true;
			return true;
		}
		else if (t->Type == TileType::TILE_PLATFORM && !m_drop)
		{
			*groundY = t->Position.y + m_map->GetTileSize();
			if ((oldPosition.y - m_halfDimension.y) >= *groundY)
				return true;
		}
	}

	return false;
}

bool Body::CheckTop(const glm::vec2& position, const glm::vec2& oldPosition, float* groundY)
{
	Line sensor = GetSensorTop(position, m_offsetVertical);
	Line oldSensor = GetSensorTop(oldPosition, m_offsetVertical);

	auto tiles = m_map->GetAdjacentTiles(oldSensor.start, sensor.end - oldSensor.start);

	std::sort(tiles.begin(), tiles.end(), [](const Tile* t1, const Tile* t2)
	{
		return t1->Position.y < t2->Position.y;
	});

	for (auto& t : tiles)
	{
		if (t->Type == TileType::TILE_SOLID)
		{
			*groundY = t->Position.y;
			return true;
		}
	}

	return false;
}

bool Body::CheckLeft(const glm::vec2& position, const glm::vec2& oldPosition, float* wallX)
{
	Line sensor = GetSensorLeft(position, m_offsetHorizontal);
	Line oldSensor = GetSensorLeft(oldPosition, m_offsetHorizontal);

	auto tiles = m_map->GetAdjacentTiles(sensor.start, oldSensor.end - sensor.start);

	std::sort(tiles.begin(), tiles.end(), [](const Tile* t1, const Tile* t2)
	{
		return t1->Position.x > t2->Position.x;
	});

	for (auto& t : tiles)
	{
		if (t->Type == TileType::TILE_SOLID)
		{
			*wallX = t->Position.x + m_map->GetTileSize();
			return true;
		}
	}

	return false;
}

bool Body::CheckRight(const glm::vec2& position, const glm::vec2& oldPosition, float* wallX)
{
	Line sensor = GetSensorRight(position, m_offsetHorizontal);
	Line oldSensor = GetSensorRight(oldPosition, m_offsetHorizontal);

	auto tiles = m_map->GetAdjacentTiles(oldSensor.start, sensor.end - oldSensor.start);

	std::sort(tiles.begin(), tiles.end(), [](const Tile* t1, const Tile* t2)
	{
		return t1->Position.x < t2->Position.x;
	});

	for (auto& t : tiles)
	{
		if (t->Type == TileType::TILE_SOLID)
		{
			*wallX = t->Position.x;
			return true;
		}
	}

	return false;
}

bool Body::CheckSlope(const glm::vec2& position, TileType slope) const
{
	Tile* tile = m_map->at(m_map->GetIndexF(position));

	if (tile == nullptr)
		return false;

	return tile->Type == slope;
}

void Body::ResolveBodyCollision(const Body& b, const glm::vec2& oldPos)
{
	glm::vec2 overlap;

	overlap.x = std::min(GetX2() - b.GetX(), b.GetX2() - GetX());
	overlap.y = std::min((oldPos.y + m_halfDimension.y) - b.GetY(), b.GetY2() - (oldPos.y - m_halfDimension.y));

	// horizontal resolve
	if (overlap.x > 0.0f && overlap.y > 0.0f)
	{
		if (m_velocity.x < 0.0f)
		{
			m_position.x += std::max(overlap.x, 0.0f);
			m_velocity.x = 0.0f;
			m_collidesLeft = true;
		}
		else if (m_velocity.x > 0.0f)
		{
			m_position.x -= std::max(overlap.x, 0.0f);
			m_velocity.x = 0.0f;
			m_collidesRight = true;
		}
	}

	overlap.x = std::min((oldPos.x + m_halfDimension.x) - b.GetX(), b.GetX2() - (oldPos.x - m_halfDimension.x));
	overlap.y = std::min(GetY2() - b.GetY(), b.GetY2() - GetY());

	// vertical resolve
	if (overlap.x > 0.0f && overlap.y > 0.0f)
	{
		if (m_velocity.y < 0.0f)
		{
			m_position.y += std::max(overlap.y, 0.0f);
			m_velocity.y = 0.0f;
			m_collidesBottom = true;
		}
		else if (m_velocity.y > 0.0f)
		{
			m_position.y -= std::max(overlap.y, 0.0f);
			m_velocity.y = 0.0f;
			m_collidesTop = true;
		}
	}
}

Line Body::GetSensorBottom(const glm::vec2& pos, const glm::vec2& offset) const
{
	return Line(pos.x - m_halfDimension.x + offset.x, pos.y - m_halfDimension.y - offset.y, pos.x + m_halfDimension.x - offset.x, pos.y - m_halfDimension.y - offset.y);
}

Line Body::GetSensorTop(const glm::vec2& pos, const glm::vec2& offset) const
{
	return Line(pos.x - m_halfDimension.x + offset.x, pos.y + m_halfDimension.y + offset.y, pos.x + m_halfDimension.x - offset.x, pos.y + m_halfDimension.y + offset.y);
}

Line Body::GetSensorLeft(const glm::vec2& pos, const glm::vec2& offset) const
{
	return Line(pos.x - m_halfDimension.x - offset.x, pos.y - m_halfDimension.y + offset.y, pos.x - m_halfDimension.x - offset.x, pos.y + m_halfDimension.y - offset.y);
}

Line Body::GetSensorRight(const glm::vec2& pos, const glm::vec2& offset) const
{
	return Line(pos.x + m_halfDimension.x + offset.x, pos.y - m_halfDimension.y + offset.y, pos.x + m_halfDimension.x + offset.x, pos.y + m_halfDimension.y - offset.y);
}

void Body::Update(float deltaTime)
{
	if (m_type == BodyType::BODY_STATIC)
		return;

	// ------------------ World Collision ----------------------------------
	if (m_onSlope && m_velocity.y <= 0.0f)
		m_velocity += m_map->GetGravity() * m_gravityScale * TILE_SLOPE_GRIP * deltaTime;
	else
		m_velocity += m_map->GetGravity() * m_gravityScale * deltaTime;


	m_offsetHorizontal = glm::vec2(m_sensorOffset, m_onSlope ? 12.0f : 2.0f);
	m_offsetVertical = glm::vec2(2.0f, m_sensorOffset);

	// check for slopes
	if (m_velocity.x < 0.0f)
	{
		// far sensors
		m_slopeDetected = CheckSlope(m_position + glm::vec2(-(m_halfDimension.x + m_map->GetTileSize() - m_sensorOffset), m_sensorOffset - m_halfDimension.y), TileType::TILE_SLOPE_LEFT);
		// near sensors
		m_slopeDetected |= CheckSlope(m_position + glm::vec2(-(m_halfDimension.x + (m_map->GetTileSize() / 2.0f) - m_sensorOffset), m_sensorOffset - m_halfDimension.y), TileType::TILE_SLOPE_LEFT);
	}
	else if (m_velocity.x > 0.0f)
	{
		// far sensors
		m_slopeDetected = CheckSlope(m_position + glm::vec2(m_halfDimension.x + m_map->GetTileSize() - m_sensorOffset, m_sensorOffset - m_halfDimension.y), TileType::TILE_SLOPE_RIGHT);
		// near sensors
		m_slopeDetected |= CheckSlope(m_position + glm::vec2(m_halfDimension.x + (m_map->GetTileSize() / 2.0f) - m_sensorOffset, m_sensorOffset - m_halfDimension.y), TileType::TILE_SLOPE_RIGHT);
	}
	else
		m_slopeDetected = false;

	glm::vec2 oldPos = m_position;

	// move first in x direction and then in y direction
	MoveX(m_velocity.x * deltaTime);
	MoveY(m_velocity.y * deltaTime);

	m_drop = false;

	// ------------------ Object Collision ---------------------------------
	for (auto& b : m_map->GetOtherBodies(this))
	{
		ResolveBodyCollision(*b, oldPos);
	}
}

void Body::Render() const
{
	//Primitives::DrawRect(m_position - m_halfDimension, m_halfDimension * 2.0f, ignis::GREEN, View::GetMat());

#if TILE_SHOW_SENSOR
	// showing the sensors (only for dynamic bodies)
	if (m_type == BODY_DYNAMIC)
	{
		Primitives::DrawLine(GetSensorBottom(m_position, m_offsetVertical), RED, Renderer::GetViewMat());
		Primitives::DrawLine(GetSensorTop(m_position, m_offsetVertical), RED, Renderer::GetViewMat());
		Primitives::DrawLine(GetSensorLeft(m_position, m_offsetHorizontal), RED, Renderer::GetViewMat());
		Primitives::DrawLine(GetSensorRight(m_position, m_offsetHorizontal), RED, Renderer::GetViewMat());

#if TILE_SHOW_SLOPE_SENSOR
		// show slope sensors
		// far sensors
		Primitives::FillCircle(m_position + glm::vec2(-(m_halfDimension.x + m_map->GetTileSize() - m_sensorOffset), m_sensorOffset - m_halfDimension.y), 2.0f, RED, Renderer::GetViewMat());
		Primitives::FillCircle(m_position + glm::vec2(m_halfDimension.x + m_map->GetTileSize() - m_sensorOffset, m_sensorOffset - m_halfDimension.y), 2.0f, RED, Renderer::GetViewMat());
		// near sensors
		Primitives::FillCircle(m_position + glm::vec2(-(m_halfDimension.x + (m_map->GetTileSize() / 2.0f) - m_sensorOffset), m_sensorOffset - m_halfDimension.y), 2.0f, RED, Renderer::GetViewMat());
		Primitives::FillCircle(m_position + glm::vec2(m_halfDimension.x + (m_map->GetTileSize() / 2.0f) - m_sensorOffset, m_sensorOffset - m_halfDimension.y), 2.0f, RED, Renderer::GetViewMat());
#endif
	}
#endif

	// center/position of the body 
	//Primitives::FillCircle(m_position, 2.0f, BLACK, Renderer::GetViewMat());
}

void Body::SetPosition(const glm::vec2& pos) { m_position = pos; }

void Body::SetVelocity(const glm::vec2& vel) { m_velocity = vel; }

void Body::Drop() { m_drop = true; }

glm::vec2 Body::GetPosition() const { return m_position; }

glm::vec2 Body::GetVelocity() const { return m_velocity; }

float Body::GetWidth() const { return m_halfDimension.x * 2.0f; }

float Body::GetHeight() const { return m_halfDimension.y * 2.0f; }

float Body::GetX() const { return m_position.x - m_halfDimension.x; }

float Body::GetX2() const { return m_position.x + m_halfDimension.x; }

float Body::GetY() const { return m_position.y - m_halfDimension.y; }

float Body::GetY2() const { return m_position.y + m_halfDimension.y; }

bool Body::CollidesBottom() const { return m_collidesBottom; }

bool Body::CollidesTop() const { return m_collidesTop; }

bool Body::CollidesLeft() const { return m_collidesLeft; }

bool Body::CollidesRight() const { return m_collidesRight; }

TileMap* Body::GetMap() const { return m_map; }

BodyType Body::GetType() const { return m_type; }
