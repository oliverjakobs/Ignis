#pragma once

#include "Core/Texture.h"

namespace ignis
{
	struct Material
	{
		Texture* Diffuse;
		Texture* Normal;
		Texture* Specular;

		float Shininess;

		Material();
		~Material();
	};
}