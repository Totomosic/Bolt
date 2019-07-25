import sys
import os
from PIL import Image
import itertools

import src.filetypes as ft
import src.xml.XMLwriter as xml
import src.res_handlers.settings_data as settings
import src.res_handlers.resource_handlers as res

IMAGE_SETTINGS_FILE = {
    "Name" : settings.SettingsAttribute(settings.TYPE_STRING, ["*"], "__FILENAME__"),
    "Wrap Mode" : settings.SettingsAttribute(settings.TYPE_STRING, ["Repeat", "ClampToEdge"], "Repeat"),
    "Min Filter" : settings.SettingsAttribute(settings.TYPE_STRING, ["Linear", "Nearest"], "Linear"),
    "Mag Filter" : settings.SettingsAttribute(settings.TYPE_STRING, ["Linear", "Nearest"], "Linear"),
    "Mipmaps" : settings.SettingsAttribute(settings.TYPE_STRING, ["Enabled", "Disabled"], "Enabled")
}

class ImageInfo:
    def __init__(self, imageObj):
        self.width = imageObj.width
        self.height = imageObj.height
        self.components = 4
        imageObj = imageObj.convert("RGBA")
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
            err.throw_error("Unable to load image data")

def convert_image_resource(input_file, output_file, settings_map):
    imageObj = Image.open(input_file).transpose(Image.FLIP_TOP_BOTTOM)
    image = ImageInfo(imageObj)

    writer = xml.XMLwriter(output_file)
    writer.begin("TEXTURE2D", { "name" : settings_map["Name"] })
    writer.begin("width", { "type" : "int" })
    writer.write_text("{}".format(image.width))
    writer.end()
    writer.begin("height", { "type" : "int" })
    writer.write_text("{}".format(image.height))
    writer.end()
    writer.begin("components", { "type" : "int" })
    writer.write_text("{}".format(image.components))
    writer.end()
    writer.begin("options", { "wrap" : settings_map["Wrap Mode"], "minification" : settings_map["Min Filter"], "magnification" : settings_map["Mag Filter"], "mipmaps" : settings_map["Mipmaps"] })
    writer.end()
    writer.begin("data", { "type" : "byte", "len" : "{}".format(len(image.data)) })
    writer.write_bytes(bytearray(image.data))
    writer.end()
    writer.end()

res.RESOURCE_HANDLERS[ft.FILE_TYPE_IMAGE] = [IMAGE_SETTINGS_FILE, convert_image_resource]
