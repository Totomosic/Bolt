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
SolutionDir = "..\\..\\"
-- Bolt Directory
BoltDir = "Bolt-Engine\\"
-- Projects Directory
ProjectsDir = "Projects\\"

-- Include directories relative to solutions directory
IncludeDirs = {}
IncludeDirs["Bolt"] = "..\\" .. BoltDir .. "Bolt-Core\\src"
IncludeDirs["GLFW"] = "..\\" .. BoltDir .. "Bolt-Core\\vendor\\GLFW\\include"
IncludeDirs["Glad"] = "..\\" .. BoltDir .. "Bolt-Core\\vendor\\Glad\\include"
IncludeDirs["ImGui"] = "..\\" .. BoltDir .. "Bolt-Core\\vendor\\ImGui"
IncludeDirs["spdlog"] = "..\\" .. BoltDir .. "Bolt-Core\\vendor\\spdlog\\include"
IncludeDirs["FreeTypeGL"] = "..\\" .. BoltDir .. "Bolt-Core\\vendor\\FreeType-GL"
IncludeDirs["FreeType"] = "..\\" .. BoltDir .. "Bolt-Core\\vendor\\FreeType\\include"
IncludeDirs["Lua"] = "..\\" .. BoltDir .. "Bolt-Core\\vendor\\Lua\\src"

include ("Tools\\ResourceConverter")
include (BoltDir .. "Bolt-Core\\vendor\\GLFW")
include (BoltDir .. "Bolt-Core\\vendor\\Glad")
include (BoltDir .. "Bolt-Core\\vendor\\ImGui")
include (BoltDir .. "Bolt-Core\\vendor\\spdlog")
include (BoltDir .. "Bolt-Core\\vendor\\FreeType")
include (BoltDir .. "Bolt-Core\\vendor\\FreeType-GL")
include (BoltDir .. "Bolt-Core\\vendor\\Lua")
include (BoltDir .. "Bolt-Core")
include (ProjectsDir .. "DND")
include (ProjectsDir .. "Aimbooster")
include (ProjectsDir .. "Blockstream")
include (ProjectsDir .. "Meteors")
include (ProjectsDir .. "Minecraft")
include (ProjectsDir .. "Battleships")
include (ProjectsDir .. "BattleshipServer")
include (ProjectsDir .. "PBRExample")