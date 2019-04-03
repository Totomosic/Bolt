import os

PREMAKE_FORMAT_STRING_TEMPLATE = """project "{0}"
    location ""
    kind "ConsoleApp"
    language "C++"
    
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
        "../Bolt-Core/external/",
        "../Bolt-Core/src/",
        "../%{{IncludeDirs.GLFW}}",
        "../%{{IncludeDirs.Glad}}",
        "../%{{IncludeDirs.ImGui}}",
        "../%{{IncludeDirs.spdlog}}",
        "src"
    }}

    libdirs
    {{
        "../Bolt-Core/external/**"
    }}

    links
    {{
        "Bolt-Core",
        "opengl32.lib",
        "FreeImage.lib",
        "freetype26d.lib",
        "freetype-gl.lib",
        "ws2_32.lib"
    }}

    filter {{ "system:windows", "configurations:debug" }}
        cppdialect "C++17"
        systemversion "latest"
        optimize "Off"

        defines
        {{
            "BLT_BUILD_STATIC",
            "BLT_PLATFORM_WINDOWS",
            "BLT_DEBUG"
        }}
    
    filter {{ "system:windows", "configurations:release" }}
        cppdialect "C++17"
        systemversion "latest"
        optimize "On"

        defines
        {{
            "BLT_BUILD_STATIC",
            "BLT_PLATFORM_WINDOWS"
        }}"""

SOLUTION_PREMAKE_FILE = "..\\premake5.lua"
PROJECTS_DIR = "..\\"

def create_dir(name):
    if not os.path.exists(name):
        os.mkdir(name)

def main():
    prj_name = input("Project Name: ")

    create_dir("{}{}".format(PROJECTS_DIR, prj_name))
    create_dir("{}{}\\src".format(PROJECTS_DIR, prj_name))
    create_dir("{}{}\\res".format(PROJECTS_DIR, prj_name))

    f = open("{}{}/premake5.lua".format(PROJECTS_DIR, prj_name), "w")
    src = PREMAKE_FORMAT_STRING_TEMPLATE.format(prj_name)
    f.write(src)
    f.close()

    f = open(SOLUTION_PREMAKE_FILE, "a")
    f.write("\ninclude (ProjectsDir .. \"{}\")".format(prj_name))
    f.close()

if __name__ == "__main__":
    main()
