#pragma once

#include "imgui/imgui.h"

struct GLFWwindow;

struct ImGuiRenderer
{
	static void Init(GLFWwindow* window);
	static void Quit();

	static void Begin();
	static void End(float width, float height);
};