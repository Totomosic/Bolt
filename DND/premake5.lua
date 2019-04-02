project "DND"
    location ""
    kind "ConsoleApp"
    language "C++"
    
    targetdir (SolutionDir .. "bin/" .. outputdir .. "/DND")
    objdir (SolutionDir .. "bin-int/" .. outputdir .. "/DND")

    pchheader "bltpch.h"
    pchsource "src/bltpch.cpp"
    
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
        "../%{IncludeDirs.GLFW}",
        "../%{IncludeDirs.Glad}",
		"../%{IncludeDirs.ImGui}",
		"../%{IncludeDirs.spdlog}",
        "src"
    }

    libdirs
    {
        "../Bolt-Core/external/**"
    }

    links
    {
        "Bolt-Core",
        "opengl32.lib",
        "FreeImage.lib",
        "freetype26d.lib",
        "freetype-gl.lib",
        "ws2_32.lib"
    }

    filter { "system:windows", "configurations:debug" }
        cppdialect "C++17"
        systemversion "latest"
        optimize "Off"

        defines
        {
            "BLT_BUILD_STATIC",
            "BLT_PLATFORM_WINDOWS",
            "BLT_DEBUG"
        }
    
    filter { "system:windows", "configurations:release" }
        cppdialect "C++17"
        systemversion "latest"
        optimize "On"

        defines
        {
            "BLT_BUILD_STATIC",
            "BLT_PLATFORM_WINDOWS"
        }