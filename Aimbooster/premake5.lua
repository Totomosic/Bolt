project "Aimbooster"
    location ""
    kind "ConsoleApp"
    language "C++"
    
    targetdir ("../bin/" .. outputdir .. "/Aimbooster")
    objdir ("../bin-int/" .. outputdir .. "/Aimbooster")
    
    files
    {
        "src/**.h",
        "src/**.cpp"
    }
    
    includedirs
    {
        "../Bolt-Core/external/",
        "../Bolt-Core/src/",
        "../Bolt-Core/external/spdlog/include/",
        "C:/Users/Jordan Morrison/AppData/Local/Programs/Python/Python37-32/include/",
        "../%{IncludeDirs.GLFW}",
        "../%{IncludeDirs.Glad}"
    }

    libdirs
    {
        "../Bolt-Core/external/**",
        "C:/Users/Jordan Morrison/AppData/Local/Programs/Python/Python37-32/libs/"
    }

    links
    {
        "Bolt-Core",
        "opengl32.lib",
        "FreeImage.lib",
        "freetype26d.lib",
        "freetype-gl.lib",
        "python37.lib",
        "ws2_32.lib"
    }

    filter { "system:windows", "configurations:debug" }
        cppdialect "C++17"
        systemversion "10.0.16299.0"
        optimize "Off"

        defines
        {
            "BLT_BUILD_STATIC",
            "BLT_PLATFORM_WINDOWS",
            "BLT_DEBUG"
        }
    
    filter { "system:windows", "configurations:release" }
        cppdialect "C++17"
        systemversion "10.0.16299.0"
        optimize "On"

        defines
        {
            "BLT_BUILD_STATIC",
            "BLT_PLATFORM_WINDOWS"
        }