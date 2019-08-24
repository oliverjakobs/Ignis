#pragma once 

#include "Buffer.h"
#include "Renderer.h"

class Ignis
{
public:
	bool LoadGL(void* loadProc, bool debug = false);
	void EnableBlend(uint sfactor, uint dfactor);
	void SetClearColor(float r, float g, float b, float a);
};