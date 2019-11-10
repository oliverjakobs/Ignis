#pragma once

#include "Maths.h"
#include <vector>

struct IntersectionPoint
{
	glm::vec2 pos;
	float angle;
};

inline std::vector<IntersectionPoint> GetVisibilityPolygonPoints(const glm::vec2& pos, std::vector<Line> edges, float range, float precision = 0.1f)
{
	std::vector<IntersectionPoint> intersections;

	std::vector<glm::vec2> vertices;

	// get all vertices
	for (auto& e : edges)
	{
		vertices.push_back(e.start);
		vertices.push_back(e.end);
	}

	// remove duplicates with a given precision
	vertices.resize(std::distance(vertices.begin(), std::unique(vertices.begin(), vertices.end(), [&](const auto& t1, const auto& t2)
	{
		return std::fabs(t1.x - t2.x) < precision && std::fabs(t1.y - t2.y) < precision;
	})));

	for (auto& v : vertices)
	{
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
			rd = range * glm::vec2(cosf(ang), sinf(ang));

			float min_t1 = INFINITY;
			glm::vec2 minPos = glm::vec2();
			float minAng = 0;
			bool valid = false;

			// Check for ray intersection with all edges
			for (auto& e : edges)
			{
				// Create line segment vector
				glm::vec2 sd = e.end - e.start;

				if (fabs(sd.x - rd.x) > 0.0f && fabs(sd.y - rd.y) > 0.0f)
				{
					// t2 is normalised distance from line segment start to line segment end of intersect point
					float t2 = (rd.x * (e.start.y - pos.y) + (rd.y * (pos.x - e.start.x))) / (sd.x * rd.y - sd.y * rd.x);
					// t1 is normalised distance from source along ray to ray length of intersect point
					float t1 = (e.start.x + sd.x * t2 - pos.x) / rd.x;

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

	// Sort intersection points by angle from source
	std::sort(intersections.begin(), intersections.end(), [&](const auto& t1, const auto& t2)
	{
		return t1.angle < t2.angle;
	});

	return intersections;
}