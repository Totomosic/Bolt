outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Bolt Directory
BoltCoreDir = "Bolt-Core/"
BoltLibDir = "BoltLib/"
ResourcesLibDir = "ResourcesLib/"
-- Resources Directory
ResourcesDir = "Bolt-Resources/"
SolutionDir = "../../"

-- Include directories relative to solution directory
IncludeDirs = {}
IncludeDirs["Bolt"] =           BoltCoreDir .. "src"
IncludeDirs["BoltLib"] =        BoltLibDir .. "src"
IncludeDirs["ResourcesLib"] =   ResourcesLibDir .. "src"
IncludeDirs["GLFW"] =           BoltCoreDir .. "vendor/GLFW/include"
IncludeDirs["Glad"] =           BoltCoreDir .. "vendor/Glad/include"
IncludeDirs["ImGui"] =          BoltCoreDir .. "vendor/ImGui"
IncludeDirs["spdlog"] =         BoltLibDir .. "vendor/spdlog/include"
IncludeDirs["FreeTypeGL"] =     BoltCoreDir .. "vendor/FreeType-GL"
IncludeDirs["FreeType"] =       BoltCoreDir .. "vendor/FreeType/include"
IncludeDirs["Lua"] =            BoltCoreDir .. "vendor/Lua/src"
IncludeDirs["Dukglue"] =        BoltCoreDir .. "vendor/Dukglue/include"

-- Library directories relative to solution directory
LibraryDirs = {}

-- Links
Links = {}