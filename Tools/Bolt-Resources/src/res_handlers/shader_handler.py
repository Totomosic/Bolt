import sys
import os

import src.filetypes as ft
import src.xml.XMLwriter as xml
import src.res_handlers.settings_data as settings
import src.res_handlers.resource_handlers as res

SHADER_SETTINGS_FILE = {
    "Name" : settings.SettingsAttribute(settings.TYPE_STRING, ["*"], "__FILENAME__")
}

def convert_shader_resource(input_file, output_file, settings_map):
    f = open(input_file, "r")
    data = f.read()
    writer = xml.XMLwriter(output_file)
    writer.begin("SHADER", { "name" : settings_map["Name"] })
    writer.begin("source", { "type" : "string" })
    writer.write_text(data)
    writer.end()
    writer.end()

res.RESOURCE_HANDLERS[ft.FILE_TYPE_SHADER] = [SHADER_SETTINGS_FILE, convert_shader_resource]