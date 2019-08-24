#pragma once 

#include "Core/Buffer.h"
#include "Core/Utility.h"

#include "Renderer.h"

class Ignis
{
public:
	Ignis();
	~Ignis();

	bool LoadGL(void* loadProc, bool debug = false);
	void EnableBlend(uint sfactor, uint dfactor);
	void SetClearColor(float r, float g, float b, float a);
};