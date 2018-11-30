"""
    For use with the Bolt C++ Engine
    Converts resource files such as PNG, JPG images and OBJ models to
    custom BRES file format
"""

from Handlers.image_handler import *

def determine_file_type(extension):
    if extension == "png" or extension == "jpg" or extension == "jpeg":
        return FILE_TYPE_IMAGE
    return FILE_TYPE_UNKNOWN

def find_resource_handler(filetype):
    return RESOURCE_HANDLERS[filetype]

def check_accepted_values(value, value_list_string):
    if len(value_list_string) == 0 or value_list_string == "*":
        return True
    value_list = value_list_string.split(',')
    return value in value_list

def get_user_value(argument_name, argument_options, arguments_dict):
    user_value = input(argument_name + " ({}), ({}), ({}): ".format(argument_options[0], argument_options[1], argument_options[2]))
    if len(user_value) == 0 and len(argument_options[2]) != 0:
        user_value = argument_options[2]
    if not check_accepted_values(user_value, argument_options[1]):
        print("Unaccepted value", user_value, "try again!")
        get_user_value(argument_name, argument_options, arguments_dict)
        return
    arguments_dict[argument_name] = user_value

def main():
    if len(sys.argv) < 1:
        print("Usage: python resource_converter.py (resource_file)")
        pause()
        return
    resource_filename = sys.argv[1]
    for i in range(2, len(sys.argv)):
        resource_filename += " " + sys.argv[i]
    filename, extension = os.path.splitext(resource_filename)

    file_type = determine_file_type(extension[1:])
    if file_type == FILE_TYPE_UNKNOWN:
        throw_error("Unknown file type with extenstion {}".format(extension))

    resourceHandler = find_resource_handler(file_type)
    arguments = {}
    for key in resourceHandler.arguments:
        if resourceHandler.arguments[key][2] == "__FILENAME__":
            resourceHandler.arguments[key][2] = os.path.splitext(os.path.basename(resource_filename))[0]
        get_user_value(key, resourceHandler.arguments[key], arguments)

    output_filename = arguments["Name"] + ".bres"
        
    resourceHandler.write_function(output_filename, resource_filename, arguments, append=False)
    
    print("Done!")
    pause()

if __name__ == "__main__":
    main()
