project "Glad"
    location ""
    kind "StaticLib"
    language "C"

    targetdir (SolutionDir .. "../../bin/" .. outputdir .. "/%{prj.name}")
    objdir (SolutionDir .. "../../bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "include/glad/glad.h",
        "include/KHR/khrplatform.h",
        "src/glad.c"
    }

    includedirs
    {
        "include"
    }

    filter "system:windows"
        systemversion "latest"
        staticruntime "On"