workspace "Ignis"
	architecture "x64"
	startproject "Ignis"

	configurations
	{
		"Debug",
		"Release"
	}

outputDir = "%{cfg.buildcfg}"

include "Ignis/packages/glfw"

project "Ignis"
	location "Ignis"
	kind "ConsoleApp"
	language "C"
	cdialect "C99"
	staticruntime "On"
	
	targetdir ("bin/" .. outputDir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputDir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.c",
		--Config
		"%{prj.name}/config.json",
		--Resources
		"%{prj.name}/res/fonts/**.ttf",
		"%{prj.name}/res/shaders/**.vert",
		"%{prj.name}/res/shaders/**.frag",
		"%{prj.name}/res/templates/**.json",
		"%{prj.name}/res/textures/**.png"
	}

	links
	{
		"GLFW",
		"opengl32"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/packages/glfw/include"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"