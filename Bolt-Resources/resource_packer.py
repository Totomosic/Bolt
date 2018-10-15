import sys
import os

from Handlers.error_handler import *

def main():
    if len(sys.argv) < 1:
        print("Usage: resource_packer.py directory_path")
        pause()
        return
    directory_name = sys.argv[1]
    output_filename = os.path.basename(directory_name) + ".pack"
    with open(output_filename, "w") as file:
        file.write("<ResourcePack>\n")
    for f in os.listdir(directory_name):
        filename = os.path.join(directory_name, f)
        print(filename)
        if not os.path.isfile(filename):
            continue
        with open(output_filename, "ab") as file:
            with open(filename, "rb") as read_file:
                data = read_file.read()
                file.write(data)
    with open(output_filename, "a") as file:
        file.write("</ResourcePack>")
    pause()

if __name__ == "__main__":
    main()
