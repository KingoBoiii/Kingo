workspace "Kingo"
    architecture 'x86_64'

    configurations {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Kingo"
    location "Kingo"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.name}/Sources/**.h",
        "%{prj.name}/Sources/**.cpp"
    }

    includedirs {
        "%{prj.name}/Vendor/spdlog/include",
        "%{prj.name}/Sources"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines {
            "KE_PLATFORM_WINDOWS",
            "KE_BUILD_DLL"
        }

        postbuildcommands {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
        }
    
    filter "configurations:Debug"
        defines "KE_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "KE_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "KE_DIST"
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
        symbols "On"

    filter "configurations:Release"
        defines "KE_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "KE_DIST"
        optimize "On"




