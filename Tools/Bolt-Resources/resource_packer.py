import os
import sys

import src.filetypes as ft
import src.error_handler as err

def Pack(input_dir, output_file):
    print("Packing...")
    print("====================================")
    with open(output_file, "w") as file:
        file.write("<ResourcePack>\n")
    for f in os.listdir(input_dir):
        filename = os.path.join(input_dir, f)
        print(filename)
        if not os.path.isfile(filename) or not filename.endswith(ft.DEFAULT_OUTPUT_EXTENSION):
            continue
        with open(output_file, "ab") as file:
            with open(filename, "rb") as read_file:
                data = read_file.read()
                file.write(data)
    with open(output_file, "a") as file:
        file.write("</ResourcePack>")
    print("====================================")

if __name__ == "__main__":
    args = sys.argv
    if len(args) <= 1:
        err.throw_error(err.ERROR_TYPE_INVALID_USAGE, "python resource_packer.py input_dir [output_file]")
        sys.exit()
    input_dir = args[1]
    output_file = os.path.dirname(input_dir) + os.path.basename(input_dir) + ft.DEFAULT_OUTPUT_PACK_EXTENSION
    if len(args) == 3:
        output_file = args[2]
    Pack(input_dir, output_file)
    err.pause()