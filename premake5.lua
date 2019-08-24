workspace "Ignis"
	startproject "Ignis"

	configurations
	{
		"Debug",
		"Release"
	}

outputDir = "%{cfg.buildcfg}"   

include "Ignis/packages/glfw"
include "Ignis/packages/glad"

project "Ignis"
	location "Ignis"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "On"
	
	targetdir ("bin/" .. outputDir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputDir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/res/shader/*.vert",
		"%{prj.name}/res/shader/*.frag"
	}

	links
	{
		"GLFW",
		"Glad",
		"opengl32"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/packages/glm",
		"%{prj.name}/packages/stb",
		"%{prj.name}/packages/glfw/include",
		"%{prj.name}/packages/glad/include",
		"%{prj.name}/packages/spdlog/include",
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"