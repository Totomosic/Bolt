project "Bolt-Core"
    location ""
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    
    targetdir ("../bin/" .. outputdir .. "/Bolt-Core")
    objdir ("../bin-int/" .. outputdir .. "/Bolt-Core")

    pchheader "bltpch.h"
    pchsource "src/bltpch.cpp"
    
    files
    {
        "src/**.h",
        "src/**.cpp"
    }
    
    includedirs
    {
        "../%{IncludeDirs.GLFW}",
        "../%{IncludeDirs.Glad}",
		"../%{IncludeDirs.ImGui}",
        "../%{IncludeDirs.spdlog}",
        "../%{IncludeDirs.FreeTypeGL}",
        "../%{IncludeDirs.FreeType}",
        "../%{IncludeDirs.Lua}",
        "../%{IncludeDirs.Dukglue}",
        "../%{IncludeDirs.BoltLib}",
        "../%{IncludeDirs.AssetsLib}",
        "src"
    }

    links
    {
        "ws2_32.lib",
        "GLFW",
        "Glad",
        "ImGui",
        "FreeType",
        "FreeType-GL",
        "Lua",
        "Dukglue",
        "BoltLib",
        "AssetsLib"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "BLT_PLATFORM_WINDOWS",
            "BLT_BUILD_STATIC",
            "_CRT_SECURE_NO_WARNINGS",
            "NOMINMAX",
            "GLEW_STATIC"
        }

    filter "system:linux"
        systemversion "latest"

        defines
        {
            "BLT_PLATFORM_LINUX",
            "BLT_BUILD_STATIC",
            "GLEW_STATIC"
        }

        links 
        {
            "stdc++fs"
        }

    filter "system:macosx"
        systemversion "latest"

        defines
        {
            "BLT_PLATFORM_MAC",
            "BLT_BUILD_STATIC",
            "GLEW_STATIC"
        }

        links 
        {
            "stdc++fs"
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