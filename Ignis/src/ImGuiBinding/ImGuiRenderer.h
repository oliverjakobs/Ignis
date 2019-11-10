#pragma once

#include "imgui.h"

struct GLFWwindow;

struct ImGuiRenderer
{
	static void Init(GLFWwindow* context);
	static void Quit();

	static void Begin();
	static void End();
};