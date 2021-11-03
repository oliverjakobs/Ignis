workspace "ignis"
	architecture "x64"
	startproject "ignis"

	configurations
	{
		"Debug",
		"OptimizedDebug",
		"Release"
	}

output_dir = "%{cfg.buildcfg}"

group "Packages"

include "packages/glfw"

group ""

project "ignis"
	kind "ConsoleApp"
	language "C"
	cdialect "C99"
	staticruntime "On"
	
	targetdir ("build/bin/" .. output_dir .. "/%{prj.name}")
	objdir ("build/bin-int/" .. output_dir .. "/%{prj.name}")

	files
	{
		--Config
		"config.ini",
		--Source
		"src/**.h",
		"src/**.c",
	}

	links
	{
		"GLFW",
		"$(VULKAN_SDK)/Lib/vulkan-1.lib"
	}

	includedirs
	{
		"src",
		"packages/glfw/include",
		"$(VULKAN_SDK)/Include"
	}

	filter "system:windows"
		systemversion "latest"
		defines { "_CRT_SECURE_NO_WARNINGS" }

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"
		
	filter "configurations:OptimizedDebug"
		runtime "Debug"
		symbols "On"
		optimize "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"