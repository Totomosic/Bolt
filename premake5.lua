workspace "Bolt"
    architecture "x64"

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
IncludeDirs["ImGui"] = "Bolt-Core\\vendor\\ImGui"
IncludeDirs["spdlog"] = "Bolt-Core\\vendor\\spdlog\\include"
IncludeDirs["FreeTypeGL"] = "Bolt-Core\\vendor\\FreeType-GL"
IncludeDirs["FreeType"] = "Bolt-Core\\vendor\\FreeType\\include"
IncludeDirs["Lua"] = "Bolt-Core\\vendor\\Lua\\src"

include (ProjectsDir .. "Tools\\ResourceConverter")
include (ProjectsDir .. "Bolt-Core\\vendor\\GLFW")
include (ProjectsDir .. "Bolt-Core\\vendor\\Glad")
include (ProjectsDir .. "Bolt-Core\\vendor\\ImGui")
include (ProjectsDir .. "Bolt-Core\\vendor\\spdlog")
include (ProjectsDir .. "Bolt-Core\\vendor\\FreeType")
include (ProjectsDir .. "Bolt-Core\\vendor\\FreeType-GL")
include (ProjectsDir .. "Bolt-Core\\vendor\\Lua")
include (ProjectsDir .. "Bolt-Core")
include (ProjectsDir .. "DND")
include (ProjectsDir .. "Aimbooster")
include (ProjectsDir .. "Blockstream")
include (ProjectsDir .. "Meteors")
include (ProjectsDir .. "Minecraft")