#include "Material.h"

#include "Ignis/Utility/Utility.h"

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

	void Material::Bind()
	{
		if (Diffuse)
			Diffuse->Bind(0);

		if (Normal)
			Normal->Bind(1);
	}

	void Material::Unbind()
	{
		if (Diffuse)
			Diffuse->Unbind();

		if (Normal)
			Normal->Unbind();
	}
}

