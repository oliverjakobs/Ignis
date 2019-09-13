#include "Material.h"

#include "Core/Utility.h"

namespace ignis
{
	Material::Material()
		: Diffuse(nullptr), Normal(nullptr)
	{
	}

	Material::~Material()
	{
		SAFE_DELETE(Diffuse);
		SAFE_DELETE(Normal);
	}
}

