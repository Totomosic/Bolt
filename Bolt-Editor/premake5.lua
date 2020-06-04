project "Bolt-Editor"
    location ""
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    
    targetdir ("../bin/" .. outputdir .. "/Bolt-Editor")
    objdir ("../bin-int/" .. outputdir .. "/Bolt-Editor")
    
    files
    {
        "src/**.h",
        "src/**.cpp"
    }
    
    includedirs
    {
        "src",
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
        "../%{IncludeDirs.Bolt}"
    }

    links
    {
        "Bolt-Core"
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
                "AssetsLib",
                "BoltLib",
                "GLFW",
                "Glad",
                "ImGui",
                "Lua",
                "Dukglue",
                "FreeType-GL",
                "FreeType",
                "stdc++fs",
                "dl",
                "X11",
                "pthread"
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