outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Solution Directory relative to a project
SolutionDir = "../../"
-- Bolt Directory
BoltDir = "Bolt-Core/"
-- Projects Directory
ProjectsDir = "Projects/"
-- Resources Directory
ResourcesDir = "Bolt-Resources/"

-- Include directories relative to solution directory
IncludeDirs = {}
IncludeDirs["Bolt"] =       BoltDir .. "src"
IncludeDirs["GLFW"] =       BoltDir .. "vendor/GLFW/include"
IncludeDirs["Glad"] =       BoltDir .. "vendor/Glad/include"
IncludeDirs["ImGui"] =      BoltDir .. "vendor/ImGui"
IncludeDirs["spdlog"] =     BoltDir .. "vendor/spdlog/include"
IncludeDirs["FreeTypeGL"] = BoltDir .. "vendor/FreeType-GL"
IncludeDirs["FreeType"] =   BoltDir .. "vendor/FreeType/include"
IncludeDirs["Lua"] =        BoltDir .. "vendor/Lua/src"
IncludeDirs["Python"] =     BoltDir .. "vendor/Python/include"

-- Library directories relative to solution directory
LibraryDirs = {}

-- Links
Links = {}