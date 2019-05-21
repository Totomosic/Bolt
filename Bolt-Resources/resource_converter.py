import os
import sys

import src.error_handler as err
import src.filetypes as ft

import src.res_handlers.resource_handlers as res
import src.res_handlers.image_handler as image
import src.res_handlers.shader_handler as shader
import src.res_handlers.obj_model_handler as obj
import src.res_handlers.settings_data as settings
import src.xml.XMLwriter as xml
import src.xml.XMLreader as reader

CREATED_FILES_ARRAY = []

def resolve_settings_macros(string, input_file, output_file, file_type):
    if string in settings.SETTINGS_MACROS:
        return settings.SETTINGS_MACROS[string](input_file, output_file, file_type)
    return string

def create_settings_file(filename, settings_map):
    writer = xml.XMLwriter(filename)
    writer.begin("RESOURCE_SETTINGS")
    for key in settings_map:
        writer.begin(key)
        writer.write_text(settings_map[key])
        writer.end()
    writer.end()
    CREATED_FILES_ARRAY.append(filename)

def query_user(name, type_string, possible_values, default_value, input_file, output_file, file_type):
    qString = "{} ({}) (".format(name, type_string)
    for v in possible_values:
        qString += v + ","
    if qString[-1] == ",":
        qString = qString[:-1]
    qString += ") ({}): ".format(resolve_settings_macros(default_value, input_file, output_file, file_type))
    usr_value = input(qString)
    if len(usr_value) == 0:
        usr_value = default_value
    if usr_value not in possible_values and settings.IGNORE_VALUE not in possible_values:
        err.throw_error(err.ERROR_TYPE_INVALID_SETTINGS, "invalid settings value " + usr_value)
        return ""
    return usr_value

def create_settings_map(settings_file, schema, allowUserInput, input_file, output_file, file_type):
    if settings_file != None:
        settings_reader = reader.XMLreader(settings_file)
        node = settings_reader.load()
        settings_map = {}
        for key in schema:
            attribute = schema[key]
            value = node.get_child(key).data
            settings_map[key] = resolve_settings_macros(value, input_file, output_file, file_type)
        return settings_map
    elif allowUserInput:
        settings_map = {}
        for key in schema:
            attribute = schema[key]
            value = query_user(key, attribute.TypeString, attribute.PossibleValues, attribute.DefaultValue, input_file, output_file, file_type)
            settings_map[key] = resolve_settings_macros(value, input_file, output_file, file_type)
        create_settings_file(os.path.splitext(output_file)[0] + ft.DEFAULT_SETTINGS_FILE_EXTENSION, settings_map)
        return settings_map
    else:
        settings_map = {}
        for key in schema:
            attribute = schema[key]
            value = attribute.DefaultValue
            settings_map[key] = resolve_settings_macros(value, input_file, output_file, file_type)
        return settings_map

def convert_resource(input_file, output_file, settings_file = None, allowUserInput = True):
    if settings_file == None:
        if os.path.exists(os.path.splitext(output_file)[0] + ft.DEFAULT_SETTINGS_FILE_EXTENSION):
            settings_file = os.path.splitext(output_file)[0] + ft.DEFAULT_SETTINGS_FILE_EXTENSION
    CREATED_FILES_ARRAY.clear()
    iFilename, iExtension = os.path.splitext(input_file)
    iFileType = ft.FILE_TYPE_UNKNOWN
    if iExtension in ft.FILE_EXTENSION_TYPE_MAP:
        iFileType = ft.FILE_EXTENSION_TYPE_MAP[iExtension]
    else:
        err.throw_error(err.ERROR_TYPE_INVALID_FILE, "invalid file with extension " + iExtension)
        return
    converter_data = res.RESOURCE_HANDLERS[iFileType]
    settings_map = create_settings_map(settings_file, converter_data[0], allowUserInput, input_file, output_file, iFileType)
    converter_data[1](input_file, output_file, settings_map)
    CREATED_FILES_ARRAY.append(output_file)
    print("=========================================================")
    print("Completed " + input_file)
    for f in CREATED_FILES_ARRAY:
        print ("Created File " + f)
    print("=========================================================")
    CREATED_FILES_ARRAY.clear()

if __name__ == "__main__":
    args = sys.argv
    if len(args) <= 1:
        err.throw_error(err.ERROR_TYPE_INVALID_USAGE, "python resource_converter.py input_file [output_file] [settings_file]")
        sys.exit()
    input_file = args[1]
    output_file = os.path.splitext(input_file)[0] + ft.DEFAULT_OUTPUT_EXTENSION
    settings_file = None
    if len(args) == 3:
        output_file = args[2]
    if len(args) == 4:
        settings_file = args[3]
    convert_resource(input_file, output_file, settings_file)