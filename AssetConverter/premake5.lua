project "AssetConverter"
    location ""
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    
    targetdir ("../bin/" .. outputdir .. "/AssetConverter")
    objdir ("../bin-int/" .. outputdir .. "/AssetConverter")
    targetname ("assets")
    
    files
    {
        "src/**.h",
        "src/**.cpp"
    }
    
    includedirs
    {
        "src",
        "vendor",
        "../%{IncludeDirs.spdlog}",
        "../%{IncludeDirs.argparse}",
        "../%{IncludeDirs.BoltLib}",
        "../%{IncludeDirs.AssetsLib}/../vendor",
        "../%{IncludeDirs.AssetsLib}",
    }

    links
    {
        "AssetsLib"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "_CRT_SECURE_NO_WARNINGS",
            "BLT_PLATFORM_WINDOWS",
            "BLT_BUILD_STATIC"
        }

    filter "system:linux"
        systemversion "latest"

        links
        {
            "BoltLib",
            "stdc++fs"
        }

        defines
        {
            "BLT_PLATFORM_LINUX",
            "BLT_BUILD_STATIC"
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