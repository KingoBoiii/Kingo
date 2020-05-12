workspace "Kingo"
    architecture 'x86_64'

    configurations {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Kingo/Vendor/GLFW/include"
IncludeDir["GLAD"] = "Kingo/Vendor/GLAD/include"
IncludeDir["imgui"] = "Kingo/Vendor/imgui"

include "Kingo/Vendor/GLFW"
include "Kingo/Vendor/GLAD"
include "Kingo/Vendor/imgui"

project "Kingo"
    location "Kingo"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "kepch.h"
    pchsource "Kingo/Sources/kepch.cpp"

    files {
        "%{prj.name}/Sources/**.h",
        "%{prj.name}/Sources/**.cpp"
    }

    includedirs {
        "%{prj.name}/Sources",
        "%{prj.name}/Vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.GLAD}",
        "%{IncludeDir.imgui}"
    }

    links {
        "GLFW",
        "GLAD",
        "imgui",
        "opengl32.lib"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines {
            "KE_PLATFORM_WINDOWS",
            "KE_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }

        postbuildcommands {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
        }
    
    filter "configurations:Debug"
        defines "KE_DEBUG"
        buildoptions "/MDd"
        symbols "On"

    filter "configurations:Release"
        defines "KE_RELEASE"
        buildoptions "/MD"
        optimize "On"

    filter "configurations:Dist"
        defines "KE_DIST"
        buildoptions "/MD"
        optimize "On"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.name}/Sources/**.h",
        "%{prj.name}/Sources/**.cpp"
    }

    includedirs {
        "Kingo/Vendor/spdlog/include",
        "Kingo/Sources"
    }

    links {
        "Kingo"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines {
            "KE_PLATFORM_WINDOWS"
        }
    
    filter "configurations:Debug"
        defines "KE_DEBUG"
        buildoptions "/MDd"
        symbols "On"

    filter "configurations:Release"
        defines "KE_RELEASE"
        buildoptions "/MD"
        optimize "On"

    filter "configurations:Dist"
        defines "KE_DIST"
        buildoptions "/MD"
        optimize "On"




