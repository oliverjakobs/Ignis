#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

inline float distance(const glm::vec2& a, const glm::vec2& b)
{
	return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

struct Line
{
	glm::vec2 start;
	glm::vec2 end;

	Line() : start(glm::vec2()), end(glm::vec2()) {}
	Line(const glm::vec2& s, const glm::vec2& e) : start(s), end(e) {}
	Line(float x1, float y1, float x2, float y2) : start(glm::vec2(x1, y1)), end(glm::vec2(x2, y2)) {}
};

struct Rect
{
	glm::vec2 pos;
	glm::vec2 dim;

	Rect() : pos(glm::vec2()), dim(glm::vec2()) {}
	Rect(float x, float y, float w, float h) : pos(glm::vec2(x, y)), dim(glm::vec2(w, h)) {}
	Rect(glm::vec2 pos, glm::vec2 dim) : pos(pos), dim(dim) {}
};

// based on an algorithm in Andre LeMothe's "Tricks of the Windows Game Programming Gurus"
inline bool LineIntersection(const Line& line1, const Line& line2, glm::vec2* out)
{
	float dl1_x = line1.end.x - line1.start.x;
	float dl1_y = line1.end.y - line1.start.y;
	float dl2_x = line2.end.x - line2.start.x;
	float dl2_y = line2.end.y - line2.start.y;

	float denom = dl1_x * dl2_y - dl2_x * dl1_y;

	if (denom == 0) return false; // Collinear

	bool denomPositive = denom > 0;

	float s02_x = line1.start.x - line2.start.x;
	float s02_y = line1.start.y - line2.start.y;

	float s_numer = dl1_x * s02_y - dl1_y * s02_x;
	float t_numer = dl2_x * s02_y - dl2_y * s02_x;

	if ((s_numer < 0) == denomPositive || (t_numer < 0) == denomPositive) return false; // No collision

	if (((s_numer > denom) == denomPositive) || ((t_numer > denom) == denomPositive)) return false; // No collision

	// Collision detected
	float t = t_numer / denom;

	if (out != nullptr)
		*out = glm::vec2(line1.start.x + (t * dl1_x), line1.start.y + (t * dl1_y));

	return true;
}