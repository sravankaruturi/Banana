workspace "Banana"
    architecture "x64"
    startproject "Sandbox"

    configurations{
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"]      = "Vendor/glfw/include"
IncludeDir["Glad"]      = "Vendor/Glad/include"
IncludeDir["ImGui"]     = "Vendor/ImGui"
IncludeDir["glm"]       = "Vendor/glm"
IncludeDir["spdlog"]    = "Vendor/spdlog/include"
IncludeDir["gtest"]     = "Vendor/googletest/googletest/include"
IncludeDir["assimp"]     = "Vendor/assimp/include"
IncludeDir["stb"]		= "Vendor/stb/include"

group "Dependencies"
include "Vendor/googletest/googletest"
include "Vendor/glfw"
include "Vendor/Glad"
include "Vendor/ImGui"

group ""

project "Engine"
    location "Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "eepch.h"
    pchsource "Engine/Src/eepch.cpp"

    files{
        "%{prj.name}/Src/**.h",
        "%{prj.name}/Src/**.cpp",
    }

    defines{
        "_CRT_SECURE_NO_WARNINGS"
    }

    includedirs{
        "%{prj.name}/Src",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
		"%{IncludeDir.assimp}",
		"%{IncludeDir.stb}",
		"Vendor/stb/include",
    }

    links{
        "GLFW",
        "Glad",
        "ImGui",
        "GoogleTest",
		"Vendor/assimp/Debug-lib/assimp-vc142-mt.lib",
        "opengl32.lib"
    }

    filter "system:windows"
        systemversion "latest"

        defines{
            "EE_PLATFORM_WINDOWS",
            "EE_BUILD_DLL",
        }

    filter "configurations:Debug"
        defines "EE_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "EE_Release"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "EE_DIST"
        runtime "Release"
        optimize "on"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"

    language "C++"
    cppdialect "C++17"

    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files{
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }

    includedirs{
        "%{IncludeDir.spdlog}",
        "Engine/Src",
        "%{IncludeDir.glm}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.gtest}",
		"%{IncludeDir.stb}"
    }

    links{
        "Engine"
    }
    
    filter "system:windows"
        systemversion "latest"

        defines{
            "EE_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "EE_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "EE_Release"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "EE_DIST"
        runtime "Release"
        optimize "on"
