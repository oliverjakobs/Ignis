#include "Material.h"

#include "Core/Utility.h"

namespace ignis
{
	Material::Material()
	{
		Diffuse = nullptr;
		Normal = nullptr;
		Specular = nullptr;

		Shininess = 0.0f;
	}

	Material::~Material()
	{
		SAFE_DELETE(Diffuse);
		SAFE_DELETE(Normal);
		SAFE_DELETE(Specular);
	}
}

