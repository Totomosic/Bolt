project "Lua"
    kind "StaticLib"
    language "C"
    staticruntime "on"
    
    targetdir (SolutionDir .. "../../bin/" .. outputdir .. "/%{prj.name}")
    objdir (SolutionDir .. "../../bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.hpp",
        "src/**.c"
    }
    
    filter "system:windows"
        systemversion "latest"        
        
    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
