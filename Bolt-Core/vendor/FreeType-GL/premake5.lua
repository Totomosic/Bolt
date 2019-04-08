project "FreeType-GL"
    kind "StaticLib"
    language "C"
    staticruntime "on"
    
    targetdir (SolutionDir .. "../../bin/" .. outputdir .. "/%{prj.name}")
    objdir (SolutionDir .. "../../bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "**.h",
        "**.c"
    }

    includedirs
    {
        "../Glad/include",
        "../../external/freetype/include/"
    }
    
    filter "system:windows"
        systemversion "latest"
        
    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
