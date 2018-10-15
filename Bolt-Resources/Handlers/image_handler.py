from PIL import Image

from Handlers.error_handler import *
from Handlers.handlers import *

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
            throw_error("Unable to load image data")

def write_image_resource(output_file, input_file, arguments, append):
    imageObj = Image.open(input_file).transpose(Image.FLIP_TOP_BOTTOM)
    image = ImageInfo(imageObj)

    writer = XMLwriter(output_file)
    writer.begin("TEXTURE2D", { "name" : arguments["Name"] })
    writer.begin("width", { "type" : "int" })
    writer.write_text("{}".format(image.width))
    writer.end()
    writer.begin("height", { "type" : "int" })
    writer.write_text("{}".format(image.height))
    writer.end()
    writer.begin("components", { "type" : "int" })
    writer.write_text("{}".format(image.components))
    writer.end()
    writer.begin("options", { "minification" : arguments["Min Filter"], "magnification" : arguments["Mag Filter"], "mipmaps" : arguments["Mipmaps"] })
    writer.end()
    writer.begin("data", { "type" : "byte", "len" : "{}".format(len(image.data)) })
    writer.write_bytes(bytearray(image.data))
    writer.end()
    writer.end()

# "Argument_Name" : [type, accepted_values, default_value]

RESOURCE_HANDLERS[FILE_TYPE_IMAGE] = ResourceHandler(write_image_resource, {
        "Name" : ["string", "*", ""],
        "Min Filter" : ["string", "linear,nearest", "linear"],
        "Mag Filter" : ["string", "linear,nearest", "linear"],
        "Mipmaps" : ["bool", "true,false", "true"]
    })
