"""
    For use with the Bolt C++ Engine
    Converts resource files such as PNG, JPG images and OBJ models to
    custom BRES file format
"""

import sys
import os
from PIL import Image
import itertools

RESOURCE_HANDLER_VERSION = "001.000"
FILE_TYPE_UNKNOWN = 0
FILE_TYPE_IMAGE = 1

class ImageInfo:
    def __init__(self, imageObj):
        self.width = imageObj.width
        self.height = imageObj.height
        self.components = 4
        imageData = list(itertools.chain(*list(imageObj.getdata())))
        if imageObj.mode == "RGB":
            self.data = []
            for i in range(0, len(imageData), 3):
                self.data.append(imageData[i])
                self.data.append(imageData[i + 1])
                self.data.append(imageData[i + 2])
                self.data.append(255)
        elif imageObj.mode == "RGBA":
            self.data = imageData
        else:
            raise Exception("Unable to load image data")
            

def pause():
    input("Enter to continue...")

def determine_file_type(extension):
    if extension == "png" or extension == "jpg" or extension == "jpeg":
        return FILE_TYPE_IMAGE
    return FILE_TYPE_UNKNOWN

def write_image_resource(output_file, input_file, resource_name):
    imageObj = Image.open(input_file)
    image = ImageInfo(imageObj)

    with open(output_file, "w") as file:
        file.write("res=IMAGE,version={},name={},".format(RESOURCE_HANDLER_VERSION, resource_name))
        file.write("w={},".format(image.width))
        file.write("h={},".format(image.height))
        file.write("c={},".format(image.components))
        file.write("len={},".format(len(image.data)))
        file.write("data=[")
    with open(output_file, "ab") as file:
        file.write(bytearray(image.data))
    with open(output_file, "a") as file:
        file.write("]")

RESOURCE_LOADERS = {
        FILE_TYPE_IMAGE : write_image_resource
    }

def main():
    if len(sys.argv) < 2:
        print("Usage: python resource_converter.py (resource_file)")
        pause()
        return
    resource_filename = sys.argv[1]
    filename, extension = os.path.splitext(resource_filename)
    output_filename = filename + ".bres"

    file_type = determine_file_type(extension[1:])
    if file_type == FILE_TYPE_UNKNOWN:
        raise Exception("Unknown file type with extenstion {}".format(extension))
    RESOURCE_LOADERS[file_type](output_filename, resource_filename, "shark_texture")
    
    pause()

if __name__ == "__main__":
    main()
