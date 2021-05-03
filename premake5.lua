project "Ignis"
	kind "StaticLib"
	language "C"
	
	targetdir ("bin/" .. outputDir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputDir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.c"
	}

	links
	{
		"opengl32"
	}

	includedirs
	{
		"src"
	}

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"

		defines
		{
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"