#include "Tile.h"

#include "Obelisk/Obelisk.h"

BodyType FromString(const std::string& str)
{
	if (obelisk::StringCompare(str, "BodyTypeStatic"))
		return BodyType::BODY_STATIC;

	if (obelisk::StringCompare(str, "BodyTypeDynamic"))
		return BodyType::BODY_DYNAMIC;

	return BodyType::BODY_STATIC;
}