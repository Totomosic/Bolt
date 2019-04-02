project "spdlog"
    kind "StaticLib"
    language "C++"
    
    targetdir (SolutionDir .. "../../bin/" .. outputdir .. "/%{prj.name}")
    objdir (SolutionDir .. "../../bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "include/**.h"
    }
    
    filter "system:windows"
        systemversion "latest"
        cppdialect "C++17"
        staticruntime "On"
        
    filter { "system:windows", "configurations:Release" }
        buildoptions "/MT"
