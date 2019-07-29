from create_premake_project import SOLUTION_PREMAKE_FILE
from create_premake_project import PROJECTS_DIR
from create_premake_project import PROJECT_INCLUDE_TEMPLATE
import os
import shutil

def main():
    prj_name = input("Project Name: ")
    project_dir = PROJECTS_DIR + prj_name
    if os.path.isdir(project_dir):
        shutil.rmtree(project_dir)
        data = ""
        with open(SOLUTION_PREMAKE_FILE, "r") as f:
            data = f.read()
            data = data.replace(PROJECT_INCLUDE_TEMPLATE.format(prj_name), "")
        with open(SOLUTION_PREMAKE_FILE, "w") as f:
            f.write(data)

    else:
        print("Invalid project name")

if __name__ == "__main__":
    main()