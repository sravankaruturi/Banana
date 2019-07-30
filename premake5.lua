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
IncludeDir["GLFW"]      = "Vendor/GLFW/include"
IncludeDir["Glad"]      = "Vendor/Glad/include"
IncludeDir["ImGui"]     = "Vendor/ImGui"
IncludeDir["glm"]       = "Vendor/glm"
IncludeDir["spdlog"]    = "Vendor/spdlog/include"
IncludeDir["gtest"]     = "Vendor/googletest/googletest/include"

group "Dependencies"
include "Vendor/googletest/googletest"
-- include "Vendor/GLFW"
-- include "Vendor/Glad"
-- include "Vendor/ImGui"

group ""

project "Engine"
    location "Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    -- pchheader "eepch.h"
    -- pchsource "Engine/Src/eepch.cpp"

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
		"%{prj.name}/Vendor/stb/include"
    }

    links{
        -- "GLFW",
        -- "Glad",
        -- "ImGui",
        "GoogleTest",
        "opengl32.lib"
    }

    filter "system:windows"
        systemversion "latest"

        defines{
            "EE_PLATFORM_WINDOWS",
            "EE_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
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
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.gtest}"
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