#pragma once

#include "Tile.h"
#include "Maths/Maths.h"

class TileMap;

class Body
{
private:
	glm::vec2 m_position;
	glm::vec2 m_halfDimension;

	glm::vec2 m_velocity;

	// -----------------------------------
	float m_sensorOffset;			// offset to the edges
	glm::vec2 m_offsetHorizontal;
	glm::vec2 m_offsetVertical;

	// CollisionState
	bool m_collidesBottom;
	bool m_collidesTop;
	bool m_collidesLeft;
	bool m_collidesRight;

	// Slopes
	bool m_onSlope;
	bool m_slopeDetected;

	float m_gravityScale;	// amplifies the magnitude of the gravity
	bool m_drop;			// should the body drop through a platform

	TileMap* m_map;
	BodyType m_type;

	// accessible through map
	Body(TileMap* map, const glm::vec2& pos, const glm::vec2& halfDim, BodyType type);

	// move in x and y direction seperatly
	void MoveX(float x);
	void MoveY(float y);

	// world collision detection
	bool CheckBottom(const glm::vec2& position, const glm::vec2& oldPosition, float* groundY);
	bool CheckTop(const glm::vec2& position, const glm::vec2& oldPosition, float* groundY);
	bool CheckLeft(const glm::vec2& position, const glm::vec2& oldPosition, float* wallX);
	bool CheckRight(const glm::vec2& position, const glm::vec2& oldPosition, float* wallX);
	bool CheckSlope(const glm::vec2& position, TileType slope) const;

	// body collision
	void ResolveBodyCollision(const Body& b, const glm::vec2& oldPos);

	// get sensors around the position
	Line GetSensorBottom(const glm::vec2& position, const glm::vec2& offset) const;
	Line GetSensorTop(const glm::vec2& position, const glm::vec2& offset) const;
	Line GetSensorLeft(const glm::vec2& position, const glm::vec2& offset) const;
	Line GetSensorRight(const glm::vec2& position, const glm::vec2& offset) const;
public:
	void Update(float deltaTime);
	void Render() const;

	void SetPosition(const glm::vec2& pos);
	void SetVelocity(const glm::vec2& vel);

	void Drop();

	glm::vec2 GetPosition() const;
	glm::vec2 GetVelocity() const;

	float GetWidth() const;
	float GetHeight() const;

	// get the edges of the body
	float GetX() const;
	float GetX2() const;
	float GetY() const;
	float GetY2() const;

	// get the collision state
	bool CollidesBottom() const;
	bool CollidesTop() const;
	bool CollidesLeft() const;
	bool CollidesRight() const;

	TileMap* GetMap() const;
	BodyType GetType() const;

	friend TileMap;
};