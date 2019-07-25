ERROR_TYPE_FILE_NOT_FOUND = 0
ERROR_TYPE_INVALID_FILE = 1
ERROR_TYPE_INVALID_USAGE = 2
ERROR_TYPE_INVALID_SETTINGS = 3

ERROR_TO_STRING_MAP = {
    ERROR_TYPE_FILE_NOT_FOUND : "FileNotFound",
    ERROR_TYPE_INVALID_FILE : "InvalidFileFormat",
    ERROR_TYPE_INVALID_USAGE : "InvalidUsage",
    ERROR_TYPE_INVALID_SETTINGS : "InvalidSettings"
}

def pause():
    input("Press enter to continue...")

def throw_error(err_type, message):
    error_str = ERROR_TO_STRING_MAP[err_type]
    err_msg = "[ERROR]: " + error_str + " " + message
    print(message)
    pause()