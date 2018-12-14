from Handlers.error_handler import *
from Handlers.handlers import *

def write_shader_resource(output_file, input_file, arguments, append):
    f = open(input_file, "r")
    data = f.read()
    writer = XMLwriter(output_file)
    writer.begin("SHADER", { "name" : arguments["Name"] })
    writer.begin("source", { "type" : "string" })
    writer.write_text(data)
    writer.end()
    writer.end()

# "Argument_Name" : [type, accepted_values, default_value]

RESOURCE_HANDLERS[FILE_TYPE_SHADER] = ResourceHandler(write_shader_resource, {
        "Name" : ["string", "*", "__FILENAME__"]
    })

def reset_shader_handler():
    RESOURCE_HANDLERS[FILE_TYPE_SHADER] = ResourceHandler(write_shader_resource, {
        "Name" : ["string", "*", "__FILENAME__"]
    })