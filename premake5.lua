workspace "Kingo"
    architecture 'x86_64'
    startproject "Sandbox"

    configurations {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Kingo/Vendor/GLFW/include"
IncludeDir["GLAD"] = "Kingo/Vendor/GLAD/include"
IncludeDir["ImGui"] = "Kingo/Vendor/ImGui"
IncludeDir["glm"] = "Kingo/Vendor/glm"
IncludeDir["stb_image"] = "Kingo/Vendor/stb_image"

group "Depedencies"
include "Kingo/Vendor/GLFW"
include "Kingo/Vendor/GLAD"
include "Kingo/Vendor/ImGui"

group ""

project "Kingo"
    location "Kingo"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "On"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "kepch.h"
    pchsource "Kingo/Sources/kepch.cpp"

    files {
        "%{prj.name}/Sources/**.h",
        "%{prj.name}/Sources/**.cpp",
        "%{prj.name}/Vendor/stb_image/**.h",
        "%{prj.name}/Vendor/stb_image/**.cpp",
        "%{prj.name}/Vendor/glm/glm/**.hpp",
        "%{prj.name}/Vendor/glm/glm/**.inl"
    }

    defines {
        "_CRT_SECURE_NO_WARNINGS"
    }

    includedirs {
        "%{prj.name}/Sources",
        "%{prj.name}/Vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.GLAD}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb_image}"
    }

    links {
        "GLFW",
        "GLAD",
        "ImGui",
        "opengl32.lib"
    }

    filter "system:windows"
        systemversion "latest"

        defines {
            "KE_PLATFORM_WINDOWS",
            "KE_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }
    
    filter "configurations:Debug"
        defines "KE_DEBUG"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines "KE_RELEASE"
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        defines "KE_DIST"
        runtime "Release"
        optimize "On"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "On"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.name}/Sources/**.h",
        "%{prj.name}/Sources/**.cpp"
    }

    includedirs {
        "Kingo/Vendor/spdlog/include",
        "Kingo/Sources",
        "Kingo/Vendor",
        "%{IncludeDir.glm}"
    }

    links {
        "Kingo"
    }

    filter "system:windows"
        systemversion "latest"

        defines {
            "KE_PLATFORM_WINDOWS"
        }
    
    filter "configurations:Debug"
        defines "KE_DEBUG"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines "KE_RELEASE"
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        defines "KE_DIST"
        runtime "Release"
        optimize "On"




