workspace "Opengl-Game"
    startproject "GameCore"

    language "C++"
    cppdialect "C++17"

    architecture "x86_64"

    configurations {"Debug", "Release"}

    filter { "configurations:Debug" }
		symbols "On"
		defines "DEBUG"

	filter { "configurations:Release" }
		optimize "On"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

function setdir()
    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
end

-- This function includes GLFW's header files
function includeGLFW()
	includedirs "Libraries/GLFW/Include"
end

function includeglm()
	includedirs "Libraries/glm"
end

function includeentt()
	includedirs "Libraries/entt/Include"
end

-- This function links statically against GLFW
function linkGLFW()
	libdirs "Libraries/GLFW/Lib"

	-- Our static lib should not link against GLFW
	filter "kind:not StaticLib"
		links "glfw3"
	filter {}
end

-- Create Glad Project

project "Glad"
    kind "StaticLib"

    setdir()

    files
    {
        "Libraries/Glad/include/glad/glad.h",
        "Libraries/Glad/include/KHR/khrplatform.h",
        "Libraries/Glad/src/glad.c"
    }

	includedirs
    {
        "Libraries/Glad/include"
    }

function includeGlad()
    includedirs "Libraries/Glad/include"
end 

function linkGlad()
    links "Glad"
end


-- Our first project, the static library
project "Lib"
	location "Projects/Lib"
	-- kind is used to indicate the type of this project.
	kind "StaticLib"

    setdir()

	pchheader "libpch.h"
	pchsource "Projects/Lib/src/libpch.cpp"

	files 
	{
		"Projects/Lib/**",
		"Libraries/glm/glm/**.hpp",
		"Libraries/glm/glm/**.inl"	
	}

	includedirs 
	{
		"Projects/Lib/src",
		"Projects/Lib/vendor",
		"Libraries/glm",
		"Libraries/tinyobjloader"
	}

    defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

	includeGLFW()
    includeGlad()
	includeentt()

function useLib()
	-- The library's public headers
	includedirs 
	{
		"Projects/Lib/src",
		"Projects/Lib/vendor"
	}

	includeglm()
	includeentt()

	-- We link against a library that's in the same workspace, so we can just
	-- use the project name - premake is really smart and will handle everything for us.
	links "Lib"

	linkGLFW()
    linkGlad()
end

project "GameCore"
	location "Projects/GameCore"
	kind "ConsoleApp"

    setdir()

	files 
	{
		"Projects/GameCore/src/**"
	}

	-- We also need the headers
	useLib()