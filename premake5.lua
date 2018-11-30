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

    includedirs
    {
        "%{prj.name}/external/",
        "%{prj.name}/src/",
        "%{prj.name}/external/GLEW/include/",
        "%{prj.name}/external/spdlog/include/",
        "C:/Users/Jordan Morrison/AppData/Local/Programs/Python/Python37/include/"
    }

    filter "system:windows"
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

project "Aimbooster"
    location "Aimbooster"
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
        "C:/Users/Jordan Morrison/AppData/Local/Programs/Python/Python37/include/"
    }

    libdirs
    {
        "Bolt-Core/external/**",
        "C:/Users/Jordan Morrison/AppData/Local/Programs/Python/Python37/libs"
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
        "ws2_32.lib"
    }

    filter "system:windows"
        cppdialect "C++17"
        systemversion "10.0.16299.0"

        defines
        {
            "BLT_BUILD_STATIC",
            "BLT_PLATFORM_WINDOWS",
            "BLT_DEBUG"
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
        "C:/Users/Jordan Morrison/AppData/Local/Programs/Python/Python37/include/"
    }

    libdirs
    {
        "Bolt-Core/external/**",
        "C:/Users/Jordan Morrison/AppData/Local/Programs/Python/Python37/libs"
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
        "ws2_32.lib"
    }

    filter "system:windows"
        cppdialect "C++17"
        systemversion "10.0.16299.0"

        defines
        {
            "BLT_BUILD_STATIC",
            "BLT_PLATFORM_WINDOWS",
            "BLT_DEBUG"
        }