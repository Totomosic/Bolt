project "FreeType-GL"
    kind "StaticLib"
    language "C"
    staticruntime "on"
    
    targetdir ("../../../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../../../bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "**.h",
        "**.c"
    }

    includedirs
    {
        "../Glad/include",
        "../freetype/include/"
    }
    
    filter "system:windows"
        systemversion "latest"
        
    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
