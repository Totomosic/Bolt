workspace "Bolt"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

include ("Paths.lua")

group("Resources")
include (ResourcesDir .. "Converter")
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
include (BoltCoreDir)