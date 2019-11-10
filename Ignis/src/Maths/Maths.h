#pragma once

#include "Geometry.h"

#include <algorithm>
#include <limits>

#define M_PI	3.14159265359f

inline bool cmp_float(float a, float b)
{
	return (fabs(a - b) < std::numeric_limits<float>::epsilon());
}

inline bool less_float(float a, float b)
{
	if (!cmp_float(a, b)) // if a < b then a != b
		return a < b;

	return false;
}

inline bool greater_float(float a, float b)
{
	if (!cmp_float(a, b)) // if a > b then a != b
		return a > b;

	return false;
}

inline bool even(int i)
{
	return (i & 1) == 0;
}