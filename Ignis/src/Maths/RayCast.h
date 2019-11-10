#pragma once

#include "Maths/Maths.h"

#include <vector>

struct Intersection
{
	glm::vec2 pos;
	Line edge;
};

typedef std::vector<Intersection> RayCastResult;

// returns all intersections with the edges along the ray (sorted by the distance to the start of the ray)
RayCastResult RayCast(const glm::vec2& pos, const glm::vec2& dir, std::vector<Line> edges, float rayMaxLength)
{
	RayCastResult result;

	for (auto& e : edges)
	{
		glm::vec2 interSec;

		if (LineIntersection(Line(pos, pos + rayMaxLength * dir), e, &interSec))
			result.push_back({ interSec, e });
	}

	if (result.empty())
		return result;

	std::sort(result.begin(), result.end(), [&](const auto& p1, const auto& p2)
	{
		return glm::distance(pos, p1.pos) < glm::distance(pos, p2.pos);
	});

	return result;
}

RayCastResult CircularRayCast(const glm::vec2& pos, std::vector<Line> edges, float count)
{
	RayCastResult result;

	const float increment = 2.0f * M_PI / count;

	float sinInc = sinf(increment);
	float cosInc = cosf(increment);

	glm::vec2 oldDir(1.0f, 0.0f);

	for (int i = 0; i < count; ++i)
	{
		// Perform rotation to avoid additional trigonometry.
		glm::vec2 dir;
		dir.x = cosInc * oldDir.x - sinInc * oldDir.y;
		dir.y = sinInc * oldDir.x + cosInc * oldDir.y;
		
		auto inter = RayCast(pos, dir, edges, 1000.0f);

		if (!inter.empty())
			result.push_back(inter.front());

		oldDir = dir;
	}

	return result;
}