import resource_converter as converter
import src.res_handlers.shader_handler as shader_handler
import src.filetypes as ft

def main():
    output_file = "shader_settings" + ft.DEFAULT_OUTPUT_EXTENSION
    converter.create_settings_map(None, shader_handler.SHADER_SETTINGS_FILE, True, output_file, output_file, ft.FILE_TYPE_SHADER)

if __name__ == "__main__":
    main()