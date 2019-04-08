project "spdlog"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    
    targetdir (SolutionDir .. "../../bin/" .. outputdir .. "/%{prj.name}")
    objdir (SolutionDir .. "../../bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "include/**.h"
    }
    
    filter "system:windows"
        systemversion "latest"
        
    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
