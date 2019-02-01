project "Bolt-Core"
    location ""
    kind "StaticLib"
    language "C++"
    
    targetdir (SolutionDir .. "bin/" .. outputdir .. "/Bolt-Core")
    objdir (SolutionDir .. "bin-int/" .. outputdir .. "/Bolt-Core")

    pchheader "Types.h"
    pchsource "src/Types.cpp"
    
    files
    {
        "src/**.h",
        "src/**.cpp",
        "src/**.hpp",
        "src/**.c"
    }
    
    includedirs
    {
        "../Bolt-Core/external/",
        "../Bolt-Core/src/",
        "../Bolt-Core/external/spdlog/include/",
        "../%{IncludeDirs.GLFW}",
        "../%{IncludeDirs.Glad}"
    }

    libdirs
    {
        "../Bolt-Core/external/**"
    }

    links
    {
        "opengl32.lib",
        "FreeImage.lib",
        "freetype26d.lib",
        "freetype-gl.lib",
        "ws2_32.lib",
        "GLFW",
        "Glad"
    }

    filter { "system:windows", "configurations:debug" }
        cppdialect "C++17"
        systemversion "latest"
        optimize "Off"

        defines
        {
            "NOMINMAX",
            "GLEW_STATIC",
            "BLT_BUILD_STATIC",
            "BLT_PLATFORM_WINDOWS",
            "BLT_DEBUG",
            "LAYERS_PER_SCENE=6",
            "GAMEOBJECTS_PER_LAYER=1000"
        }
    
    filter { "system:windows", "configurations:release" }
        cppdialect "C++17"
        systemversion "latest"
        optimize "On"

        defines
        {
            "NOMINMAX",
            "GLEW_STATIC",
            "BLT_BUILD_STATIC",
            "BLT_PLATFORM_WINDOWS",
            "LAYERS_PER_SCENE=6",
            "GAMEOBJECTS_PER_LAYER=1000"
        }