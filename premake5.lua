workspace "Bolt"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

    flags
    {
        "MultiProcessorCompile"
    }

include ("Paths.lua")

group ("Bolt/Vendor")
include (BoltCoreDir .. "vendor/GLFW")
include (BoltCoreDir .. "vendor/Glad")
include (BoltCoreDir .. "vendor/ImGui")
include (BoltLibDir .. "vendor/spdlog")
include (BoltCoreDir .. "vendor/FreeType")
include (BoltCoreDir .. "vendor/FreeType-GL")
include (BoltCoreDir .. "vendor/Lua")
include (BoltCoreDir .. "vendor/Dukglue")
group ("Bolt")
include (BoltLibDir)
group("Assets")
include (AssetsLibDir)
include (AssetsDir)
group ("Bolt")
include (BoltCoreDir)