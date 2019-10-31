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
include (BoltDir .. "vendor/GLFW")
include (BoltDir .. "vendor/Glad")
include (BoltDir .. "vendor/ImGui")
include (BoltDir .. "vendor/spdlog")
include (BoltDir .. "vendor/FreeType")
include (BoltDir .. "vendor/FreeType-GL")
include (BoltDir .. "vendor/Lua")
group("Bolt/Vendor/Python")
include (BoltDir .. "vendor/Python/cpython/PCBuild")
group ("Bolt")
include (BoltDir)
group ("Projects")
include (ProjectsDir .. "DND")
include (ProjectsDir .. "Aimbooster")
include (ProjectsDir .. "Blockstream")
include (ProjectsDir .. "Meteors")
include (ProjectsDir .. "Minecraft")
include (ProjectsDir .. "PBRExample")