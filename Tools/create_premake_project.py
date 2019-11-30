import os
import shutil

PREMAKE_FORMAT_STRING_TEMPLATE = """project "{0}"
    location ""
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    
    targetdir (SolutionDir .. "bin/" .. outputdir .. "/{0}")
    objdir (SolutionDir .. "bin-int/" .. outputdir .. "/{0}")
    
    files
    {{
        "src/**.h",
        "src/**.cpp",
        "src/**.hpp",
        "src/**.c"
    }}
    
    includedirs
    {{
        "../../%{{IncludeDirs.Bolt}}",
        "../../%{{IncludeDirs.GLFW}}",
        "../../%{{IncludeDirs.Glad}}",
        "../../%{{IncludeDirs.ImGui}}",
        "../../%{{IncludeDirs.spdlog}}",
        "../../%{{IncludeDirs.FreeType}}",
        "../../%{{IncludeDirs.FreeTypeGL}}",
        "../../%{{IncludeDirs.Lua}}",
        "src"
    }}

    links
    {{
        "Bolt-Core",
        "opengl32.lib",
        "ws2_32.lib"
    }}

    filter "system:windows"
        systemversion "latest"

        defines
        {{
            "BLT_PLATFORM_WINDOWS",
            "BLT_BUILD_STATIC"
        }}

    filter "configurations:Debug"
        defines "BLT_DEBUG"
        runtime "Debug"
        symbols "on"

    filter \"configurations:Release\"
        defines \"BLT_RELEASE\"
        runtime \"Release\"
        optimize \"on\"

    filter "configurations:Dist"
        defines \"BLT_DIST\"
        runtime \"Release\"
        optimize \"on\""""

SOLUTION_PREMAKE_FILE = "..\\premake5.lua"
PROJECTS_DIR = "..\\Projects\\"
PROJECT_INCLUDE_TEMPLATE = "\ninclude (ProjectsDir .. \"{}\")"
BUILD_RESOURCES_FILE = "BuildResources.bat"

def create_dir(name):
    if not os.path.exists(name):
        os.mkdir(name)

def main():
    prj_name = input("Project Name: ")
    res_dir = "{}{}\\res".format(PROJECTS_DIR, prj_name)

    create_dir("{}{}".format(PROJECTS_DIR, prj_name))
    create_dir("{}{}\\src".format(PROJECTS_DIR, prj_name))
    create_dir(res_dir)
    create_dir("{}{}\\res\\Resources".format(PROJECTS_DIR, prj_name))

    shutil.copyfile(BUILD_RESOURCES_FILE, res_dir + "\\" + BUILD_RESOURCES_FILE)

    f = open("{}{}/premake5.lua".format(PROJECTS_DIR, prj_name), "w")
    src = PREMAKE_FORMAT_STRING_TEMPLATE.format(prj_name)
    f.write(src)
    f.close()

    f = open(SOLUTION_PREMAKE_FILE, "a")
    f.write(PROJECT_INCLUDE_TEMPLATE.format(prj_name))
    f.close()

if __name__ == "__main__":
    main()
