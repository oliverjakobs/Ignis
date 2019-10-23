// dear imgui: Renderer for modern OpenGL with shaders / programmatic pipeline
// - Desktop GL: 3.x 4.x
// This needs to be used along with a Platform Binding (e.g. GLFW, SDL, Win32, custom..)

// Implemented features:
//  [X] Renderer: User texture binding. Use 'GLuint' OpenGL texture identifier as void*/ImTextureID. Read the FAQ about ImTextureID in imgui.cpp.
//  [X] Renderer: Multi-viewport support. Enable with 'io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable'.
//  [x] Renderer: Desktop GL only: Support for large meshes (64k+ vertices) with 16-bits indices.

// If you are new to dear imgui, read examples/README.txt and read the documentation at the top of imgui.cpp.
// https://github.com/ocornut/imgui

#pragma once

#include <imgui/imgui.h>
#include <glad/glad.h>

bool ImGuiBinding_Init(const char* glsl_version = nullptr);
void ImGuiBinding_Shutdown();
void ImGuiBinding_NewFrame();
void ImGuiBinding_RenderDrawData(ImDrawData* draw_data);

// Called by Init/NewFrame/Shutdown
bool ImGuiBinding_CreateFontsTexture();
void ImGuiBinding_DestroyFontsTexture();
bool ImGuiBinding_CreateDeviceObjects();
void ImGuiBinding_DestroyDeviceObjects();
