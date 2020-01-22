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
group ("Bolt")
include (BoltLibDir)
group("Resources")
include (ResourcesLibDir)
include (ResourcesDir)
group ("Bolt")
include (BoltCoreDir)