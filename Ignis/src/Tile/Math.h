#pragma once

#include "Tile.h"

#include <algorithm>

namespace tile
{
	struct Line
	{
		glm::vec2 Start;
		glm::vec2 End;

		Line() : Start(glm::vec2()), End(glm::vec2()) {}
		Line(const glm::vec2& s, const glm::vec2& e) : Start(s), End(e) {}
		Line(float sx, float sy, float ex, float ey) : Start(glm::vec2(sx, sy)), End(glm::vec2(ex, ey)) {}

		operator glm::vec4() const { return {Start, End}; }
	};

	std::vector<glm::vec3> Visibility(const glm::vec2& pos, std::vector<Line> edges);
}