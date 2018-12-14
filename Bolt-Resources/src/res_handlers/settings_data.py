import os

def resolve_filename_macro(input_file, output_file, resource_type):
    return os.path.splitext(os.path.basename(input_file))[0]

FILENAME_MACRO = "__FILENAME__"

TYPE_STRING = "string"
TYPE_INT = "int"
TYPE_FLOAT = "float"
TYPE_BOOL = "bool"

IGNORE_VALUE = "*"
SETTINGS_MACROS = {
    FILENAME_MACRO : resolve_filename_macro
}

class SettingsAttribute:
    def __init__(self, type_string, possible_values, default_value):
        self.TypeString = type_string
        self.PossibleValues = possible_values
        self.DefaultValue = default_value