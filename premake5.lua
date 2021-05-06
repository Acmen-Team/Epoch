workspace "Epoch"
	architecture "x64"
	startproject "Epoch"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder(solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Epoch/vendor/GLFW/include"
IncludeDir["Glad"] = "Epoch/vendor/Glad/include"
IncludeDir["ImGui"] = "Epoch/vendor/imgui"
IncludeDir["glm"] = "Epoch/vendor/glm"
IncludeDir["stb_image"] = "Epoch/vendor/stb_image"
IncludeDir["tinyobjloader"] = "Epoch/vendor/tinyobjloader"

group "Dependencies"
	include "Epoch/vendor/GLFW"
	include "Epoch/vendor/Glad"
	include "Epoch/vendor/imgui"
group ""

project "Epoch"
	location "Epoch"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "On"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "eppch.h"
	pchsource "Epoch/src/eppch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/tinyobjloader/**.h"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.tinyobjloader}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"EP_ENABLE_ASSERTS"
		}

	filter "configurations:Debug"
		defines "EP_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "EP_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "EP_Dist"
		runtime "Release"
		optimize "On"