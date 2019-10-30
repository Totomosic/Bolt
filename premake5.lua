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
-- Bolt Directory
BoltDir = "Bolt-Core\\"
-- Projects Directory
ProjectsDir = "Projects\\"

-- Include directories relative to projects directory
IncludeDirs = {}
IncludeDirs["Bolt"] =       "..\\" .. BoltDir .. "src"
IncludeDirs["GLFW"] =       "..\\" .. BoltDir .. "vendor\\GLFW\\include"
IncludeDirs["Glad"] =       "..\\" .. BoltDir .. "vendor\\Glad\\include"
IncludeDirs["ImGui"] =      "..\\" .. BoltDir .. "vendor\\ImGui"
IncludeDirs["spdlog"] =     "..\\" .. BoltDir .. "vendor\\spdlog\\include"
IncludeDirs["FreeTypeGL"] = "..\\" .. BoltDir .. "vendor\\FreeType-GL"
IncludeDirs["FreeType"] =   "..\\" .. BoltDir .. "vendor\\FreeType\\include"
IncludeDirs["Lua"] =        "..\\" .. BoltDir .. "vendor\\Lua\\src"
IncludeDirs["Python"] =     "..\\" .. BoltDir .. "vendor\\Python\\include"

-- Library directories relative to projects directory
LibraryDirs = {}
LibraryDirs["Python"] =     "..\\" .. BoltDir .. "Bolt-Core\\vendor\\Python\\lib"

-- Links
Links = {}

group ("Bolt/Vendor")
include (BoltDir .. "vendor\\GLFW")
include (BoltDir .. "vendor\\Glad")
include (BoltDir .. "vendor\\ImGui")
include (BoltDir .. "vendor\\spdlog")
include (BoltDir .. "vendor\\FreeType")
include (BoltDir .. "vendor\\FreeType-GL")
include (BoltDir .. "vendor\\Lua")
group("Bolt/Vendor/Python")
include (BoltDir .. "vendor\\Python\\cpython\\PCBuild")
group ("Bolt")
include (BoltDir)
group ("Projects")
include (ProjectsDir .. "DND")
include (ProjectsDir .. "Aimbooster")
include (ProjectsDir .. "Blockstream")
include (ProjectsDir .. "Meteors")
include (ProjectsDir .. "Minecraft")
include (ProjectsDir .. "PBRExample")