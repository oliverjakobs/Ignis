#include "Algorithm.h"

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
}