outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Bolt Directory
BoltCoreDir = "Bolt-Core/"
BoltLibDir = "BoltLib/"
AssetsLibDir = "AssetsLib/"
-- Resources Directory
AssetsDir = "AssetConverter/"
SolutionDir = "../../"

-- Include directories relative to solution directory
IncludeDirs = {}
IncludeDirs["Bolt"] =           BoltCoreDir .. "src"
IncludeDirs["BoltLib"] =        BoltLibDir .. "src"
IncludeDirs["AssetsLib"] =      AssetsLibDir .. "src"
IncludeDirs["GLFW"] =           BoltCoreDir .. "vendor/GLFW/include"
IncludeDirs["Glad"] =           BoltCoreDir .. "vendor/Glad/include"
IncludeDirs["ImGui"] =          BoltCoreDir .. "vendor/ImGui"
IncludeDirs["spdlog"] =         BoltLibDir .. "vendor/spdlog/include"
IncludeDirs["argparse"] =       BoltLibDir .. "vendor/argparse"
IncludeDirs["FreeTypeGL"] =     BoltCoreDir .. "vendor/FreeType-GL"
IncludeDirs["FreeType"] =       BoltCoreDir .. "vendor/FreeType/include"
IncludeDirs["Lua"] =            BoltCoreDir .. "vendor/Lua/src"
IncludeDirs["Dukglue"] =        BoltCoreDir .. "vendor/Dukglue/include"

-- Library directories relative to solution directory
LibraryDirs = {}

-- Links
Links = {}