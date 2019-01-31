workspace "Bolt"
    architecture "x86"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Solution Directory
SolutionDir = "..\\"
-- Projects Directory
ProjectsDir = ""

-- Include directories relative to solutions directory
IncludeDirs = {}
IncludeDirs["GLFW"] = "Bolt-Core\\vendor\\GLFW\\include"
IncludeDirs["Glad"] = "Bolt-Core\\vendor\\Glad\\include"

include (ProjectsDir .. "Bolt-Core\\vendor\\GLFW\\premake5.lua")
include (ProjectsDir .. "Bolt-Core\\vendor\\Glad")
include (ProjectsDir .. "Bolt-Core")
include (ProjectsDir .. "DND")
include (ProjectsDir .. "Aimbooster")
include (ProjectsDir .. "Blockstream")
include (ProjectsDir .. "NatureScene")