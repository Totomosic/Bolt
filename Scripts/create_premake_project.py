import os

PREMAKE_FORMAT_STRING_TEMPLATE = """project "{0}"
    location ""
    kind "ConsoleApp"
    language "C++"
    
    targetdir ("../bin/" .. outputdir .. "/{0}")
    objdir ("../bin-int/" .. outputdir .. "/{0}")
    
    files
    {{
        "src/**.h",
        "src/**.cpp"
    }}
    
    includedirs
    {{
        "../Bolt-Core/external/",
        "../Bolt-Core/src/",
        "../Bolt-Core/external/spdlog/include/",
        "C:/Users/Jordan Morrison/AppData/Local/Programs/Python/Python37-32/include/",
        "../%{{IncludeDirs.GLFW}}",
        "../%{{IncludeDirs.Glad}}"
    }}

    libdirs
    {{
        "../Bolt-Core/external/**",
        "C:/Users/Jordan Morrison/AppData/Local/Programs/Python/Python37-32/libs/"
    }}

    links
    {{
        "Bolt-Core",
        "opengl32.lib",
        "FreeImage.lib",
        "freetype26d.lib",
        "freetype-gl.lib",
        "python37.lib",
        "ws2_32.lib"
    }}

    filter {{ "system:windows", "configurations:debug" }}
        cppdialect "C++17"
        systemversion "10.0.16299.0"
        optimize "Off"

        defines
        {{
            "BLT_BUILD_STATIC",
            "BLT_PLATFORM_WINDOWS",
            "BLT_DEBUG"
        }}
    
    filter {{ "system:windows", "configurations:release" }}
        cppdialect "C++17"
        systemversion "10.0.16299.0"
        optimize "On"

        defines
        {{
            "BLT_BUILD_STATIC",
            "BLT_PLATFORM_WINDOWS"
        }}"""

SOLUTION_PREMAKE_FILE = "../premake5.lua"

def create_dir(name):
    if not os.path.exists(name):
        os.mkdir(name)

def main():
    prj_name = input("Project Name: ")

    create_dir("../{}".format(prj_name))
    create_dir("../{}/src".format(prj_name))
    create_dir("../{}/res".format(prj_name))

    f = open("../{}/premake5.lua".format(prj_name), "w")
    src = PREMAKE_FORMAT_STRING_TEMPLATE.format(prj_name)
    f.write(src)
    f.close()

    f = open(SOLUTION_PREMAKE_FILE, "a")
    f.write("\ninclude \"{}\"".format(prj_name))
    f.close()

if __name__ == "__main__":
    main()