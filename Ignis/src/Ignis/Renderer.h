#pragma once 

class Renderer
{
public:
	Renderer() = default;
	~Renderer();

	bool Init(void* loadProc, bool debug = false);
};