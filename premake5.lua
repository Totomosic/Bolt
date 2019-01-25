workspace "Bolt-Core"
    architecture "x86"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to solutions directory
IncludeDirs = {}
IncludeDirs["GLFW"] = "Bolt-Core/vendor/GLFW/include"
IncludeDirs["Glad"] = "Bolt-Core/vendor/Glad/include"

include "Bolt-Core/vendor/GLFW"
include "Bolt-Core/vendor/Glad"

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
    pchsource "Bolt-Core/src/Types.cpp"

    includedirs
    {
        "%{prj.name}/external/",
        "%{prj.name}/src/",
        "%{prj.name}/external/spdlog/include/",
        "C:/Users/Jordan Morrison/AppData/Local/Programs/Python/Python37-32/include/",
        "%{IncludeDirs.Glad}",
        "%{IncludeDirs.GLFW}"
    }

    links
    {
        "Glad",
        "GLFW"
    }

    filter { "system:windows", "configurations:debug" }
        cppdialect "C++17"
        systemversion "latest"

        defines
        {
            "NOMINMAX",
            "BLT_PLATFORM_WINDOWS",
            "BLT_BUILD_STATIC",
            "BLT_DEBUG",
            "GAMEOBJECTS_PER_LAYER=1000",
            "LAYERS_PER_SCENE=6"
        }

    filter { "system:windows", "configurations:release" }
        cppdialect "C++17"
        systemversion "latest"

        defines
        {
            "NOMINMAX",
            "BLT_PLATFORM_WINDOWS",
            "BLT_BUILD_STATIC",
            "GAMEOBJECTS_PER_LAYER=1000",
            "LAYERS_PER_SCENE=6"
        }

include "Aimbooster"
include "Blockstream"
include "DND"
include "NatureScene"