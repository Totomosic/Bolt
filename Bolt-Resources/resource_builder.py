import sys
import os

import resource_converter
import resource_packer

from Handlers.error_handler import *

def main(cmd_args = None):
    if len(sys.argv) <= 0 and cmd_args == None:
        print("Usage: python resource_builder.py (directory)")
        pause()
        return
    res_dir = sys.argv[1]
    output_file = input("output filename: (resources.pack) ")
    if len(output_file) == 0:
        output_file = "resources.pack"
    int_dir = os.path.join(res_dir, output_file.split(".")[0])

    if not os.path.exists(int_dir):
        os.makedirs(int_dir)
    else:
        for f in os.listdir(int_dir):
            fname = os.path.join(int_dir, f)
            if os.path.isfile(fname):
                os.remove(fname)
    paths = os.listdir(res_dir)
    for f in paths:
        fname = os.path.join(res_dir, f)
        if not os.path.isfile(fname):
            continue
        resource_converter.main(["", fname])
        os.rename(os.path.join(res_dir, os.path.splitext(f)[0] + ".bres"), os.path.join(int_dir, os.path.splitext(f)[0] + ".bres"))
    resource_packer.main(["", int_dir])
    outfile = os.path.join(os.path.dirname(res_dir), output_file)
    if os.path.exists(outfile):
        os.remove(outfile)
    os.rename(os.path.join(res_dir, output_file), outfile)
    print("Done!")
    if cmd_args == None:
        pause()

if __name__ == "__main__":
    main()