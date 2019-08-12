project "Bolt-Core"
    location ""
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    
    targetdir (SolutionDir .. "bin/" .. outputdir .. "/Bolt-Core")
    objdir (SolutionDir .. "bin-int/" .. outputdir .. "/Bolt-Core")

    pchheader "bltpch.h"
    pchsource "src/bltpch.cpp"
    
    files
    {
        "src/**.h",
        "src/**.cpp"
    }
    
    includedirs
    {
        "../Bolt-Core/external/",
        "../%{IncludeDirs.GLFW}",
        "../%{IncludeDirs.Glad}",
		"../%{IncludeDirs.ImGui}",
        "../%{IncludeDirs.spdlog}",
        "../%{IncludeDirs.FreeTypeGL}",
        "../%{IncludeDirs.FreeType}",
        "../%{IncludeDirs.Lua}",
        "src"
    }

    links
    {
        "opengl32.lib",
        "ws2_32.lib",
        "GLFW",
        "Glad",
        "ImGui",
        "FreeType",
        "FreeType-GL",
        "Lua"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "BLT_PLATFORM_WINDOWS",
            "BLT_BUILD_STATIC",
            "_CRT_SECURE_NO_WARNINGS",
            "NOMINMAX",
            "GLEW_STATIC",
            "LAYERS_PER_SCENE=4",
            "GAMEOBJECTS_PER_LAYER=2000"
        }

    filter "configurations:Debug"
        defines "BLT_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "BLT_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "BLT_DIST"
        runtime "Release"
        optimize "on"