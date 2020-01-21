project "Resource-Converter"
    location ""
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    
    targetdir ("../bin/" .. outputdir .. "/Resource-Converter")
    objdir ("../bin-int/" .. outputdir .. "/Resource-Converter")
    
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
        "../%{IncludeDirs.BoltLib}",
        "../%{IncludeDirs.ResourcesLib}",
    }

    links
    {
        "ResourcesLib"
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