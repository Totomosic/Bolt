project "ResourcesLib"
    location ""
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    
    targetdir ("../bin/" .. outputdir .. "/ResourcesLib")
    objdir ("../bin-int/" .. outputdir .. "/ResourcesLib")

    files
    {
        "src/**.h",
        "src/**.cpp"
    }
    
    includedirs
    {
        "../%{IncludeDirs.spdlog}",
        "../%{IncludeDirs.BoltLib}",
        "vendor",
        "src"
    }

    links
    {
        "BoltLib"
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