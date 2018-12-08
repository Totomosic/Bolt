workspace "Bolt-Core"
    architecture "x86"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Bolt-Core"
    location "Bolt-Core"
    kind "StaticLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.glsl",
        "%{prj.name}/src/**.glh"
    }

    pchheader "Types.h"
    pchsource "Types.cpp"
    
    forceincludes 
    {
        "Types.h"
    }

    includedirs
    {
        "%{prj.name}/external/",
        "%{prj.name}/src/",
        "%{prj.name}/external/GLEW/include/",
        "%{prj.name}/external/spdlog/include/",
        "C:/Users/Jordan Morrison/AppData/Local/Programs/Python/Python37-32/include/"
    }

    filter { "system:windows", "configurations:debug" }
        cppdialect "C++17"
        systemversion "10.0.16299.0"

        defines
        {
            "NOMINMAX",
            "GLEW_STATIC",
            "BLT_PLATFORM_WINDOWS",
            "BLT_BUILD_STATIC",
            "BLT_DEBUG",
            "GAMEOBJECTS_PER_LAYER=10000",
            "LAYERS_PER_SCENE=6"
        }

    filter { "system:windows", "configurations:release" }
        cppdialect "C++17"
        systemversion "10.0.16299.0"

        defines
        {
            "NOMINMAX",
            "GLEW_STATIC",
            "BLT_PLATFORM_WINDOWS",
            "BLT_BUILD_STATIC",
            "GAMEOBJECTS_PER_LAYER=10000",
            "LAYERS_PER_SCENE=6"
        }

project "Aimbooster"
    location "Aimbooster"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "Bolt-Core/external/",
        "Bolt-Core/src/",
        "Bolt-Core/external/GLEW/include/",
        "Bolt-Core/external/spdlog/include/",
        "C:/Users/Jordan Morrison/AppData/Local/Programs/Python/Python37-32/include/"
    }

    libdirs
    {
        "Bolt-Core/external/**",
        "C:/Users/Jordan Morrison/AppData/Local/Programs/Python/Python37-32/libs/"
    }

    links
    {
        "Bolt-Core",
        "opengl32.lib",
        "glew32s.lib",
        "glfw3.lib",
        "FreeImage.lib",
        "freetype26d.lib",
        "freetype-gl.lib",
        "python37.lib",
        "ws2_32.lib"
    }

    filter { "system:windows", "configurations:debug" }
        cppdialect "C++17"
        systemversion "10.0.16299.0"
        kind "ConsoleApp"
        optimize "Off"

        defines
        {
            "BLT_BUILD_STATIC",
            "BLT_PLATFORM_WINDOWS",
            "BLT_DEBUG"
        }
    
    filter { "system:windows", "configurations:release" }
        cppdialect "C++17"
        systemversion "10.0.16299.0"
        kind "ConsoleApp"
        optimize "On"

        defines
        {
            "BLT_BUILD_STATIC",
            "BLT_PLATFORM_WINDOWS"
        }

    filter { "system:windows", "configurations:dist" }
        cppdialect "C++17"
        systemversion "10.0.16299.0"
        kind "ConsoleApp"
        optimize "On"

        defines
        {
            "BLT_BUILD_STATIC",
            "BLT_PLATFORM_WINDOWS"
        }

project "NatureScene"
location "NatureScene"
kind "ConsoleApp"
language "C++"

targetdir ("bin/" .. outputdir .. "/%{prj.name}")
objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

files
{
    "%{prj.name}/src/**.h",
    "%{prj.name}/src/**.cpp"
}

includedirs
{
    "Bolt-Core/external/",
    "Bolt-Core/src/",
    "Bolt-Core/external/GLEW/include/",
    "Bolt-Core/external/spdlog/include/",
    "C:/Users/Jordan Morrison/AppData/Local/Programs/Python/Python37-32/include/"
}

libdirs
{
    "Bolt-Core/external/**",
    "C:/Users/Jordan Morrison/AppData/Local/Programs/Python/Python37-32/libs/"
}

links
{
    "Bolt-Core",
    "opengl32.lib",
    "glew32s.lib",
    "glfw3.lib",
    "FreeImage.lib",
    "freetype26d.lib",
    "freetype-gl.lib",
    "python37.lib",
    "ws2_32.lib"
}

filter { "system:windows", "configurations:debug" }
    cppdialect "C++17"
    systemversion "10.0.16299.0"
    optimize "Off"

    defines
    {
        "BLT_BUILD_STATIC",
        "BLT_PLATFORM_WINDOWS",
        "BLT_DEBUG"
    }

filter { "system:windows", "configurations:release" }
    cppdialect "C++17"
    systemversion "10.0.16299.0"
    optimize "On"

    defines
    {
        "BLT_BUILD_STATIC",
        "BLT_PLATFORM_WINDOWS"
    }

project "Blockstream"
    location "Blockstream"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "Bolt-Core/external/",
        "Bolt-Core/src/",
        "Bolt-Core/external/GLEW/include/",
        "Bolt-Core/external/spdlog/include/",
        "C:/Users/Jordan Morrison/AppData/Local/Programs/Python/Python37-32/include/"
    }

    libdirs
    {
        "Bolt-Core/external/**",
        "C:/Users/Jordan Morrison/AppData/Local/Programs/Python/Python37-32/libs/"
    }

    links
    {
        "Bolt-Core",
        "opengl32.lib",
        "glew32s.lib",
        "glfw3.lib",
        "FreeImage.lib",
        "freetype26d.lib",
        "freetype-gl.lib",
        "python37.lib",
        "ws2_32.lib"
    }

    filter { "system:windows", "configurations:debug" }
        cppdialect "C++17"
        systemversion "10.0.16299.0"
        optimize "Off"

        defines
        {
            "BLT_BUILD_STATIC",
            "BLT_PLATFORM_WINDOWS",
            "BLT_DEBUG"
        }
    
    filter { "system:windows", "configurations:release" }
        cppdialect "C++17"
        systemversion "10.0.16299.0"
        optimize "On"

        defines
        {
            "BLT_BUILD_STATIC",
            "BLT_PLATFORM_WINDOWS"
        }