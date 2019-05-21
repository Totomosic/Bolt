import os
import sys

import src.filetypes as ft
import src.error_handler as err
import resource_converter as converter
import resource_packer as packer

def Build(input_dir, output_file, int_dir = None):
    if int_dir == None:
        int_dir = os.path.join(input_dir, os.path.basename(os.path.splitext(output_file)[0]))
    if not os.path.exists(int_dir):
        os.makedirs(int_dir)
    else:
        for f in os.listdir(int_dir):
            filename = os.path.join(int_dir, f)
            if os.path.isfile(filename) and filename.endswith(ft.DEFAULT_OUTPUT_EXTENSION):
                os.remove(filename)
    for f in os.listdir(input_dir):
        filename = os.path.join(input_dir, f)
        if os.path.isfile(filename):
            converter.convert_resource(filename, os.path.join(int_dir, os.path.splitext(f)[0] + ft.DEFAULT_OUTPUT_EXTENSION), None)
    packer.Pack(int_dir, output_file)

if __name__ == "__main__":
    try:
        args = sys.argv
        if len(args) <= 0:
            err.throw_error(err.ERROR_TYPE_INVALID_USAGE, "python resource_builder.py input_directory [output_file] [intermediate_dir]")
            sys.exit()
        input_dir = args[1]
        output_file = os.path.dirname(input_dir) + os.path.basename(input_dir) + ft.DEFAULT_OUTPUT_PACK_EXTENSION
        int_dir = None
        if len(args) == 3:
            output_file = args[2]
        if len(args) == 4:
            int_dir = args[3]
        Build(input_dir, output_file, int_dir)
        err.pause()
    except Exception as e:
        print(e)
        err.pause()