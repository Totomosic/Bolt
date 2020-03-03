project "Dukglue"
    kind "StaticLib"
    language "C"
    staticruntime "on"
    
    targetdir ("../../../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../../../bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "include/*.h",
        "include/*.c",
        "include/dukglue/*.h"
    }

    includedirs
    {
        "include/duktape",
        "include"
    }
    
    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
