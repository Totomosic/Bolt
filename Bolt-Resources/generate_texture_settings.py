import resource_converter as converter
import src.res_handlers.image_handler as image_handler
import src.filetypes as ft

def main():
    output_file = "texture_settings" + ft.DEFAULT_OUTPUT_EXTENSION
    converter.create_settings_map(None, image_handler.IMAGE_SETTINGS_FILE, True, output_file, output_file, ft.FILE_TYPE_IMAGE)

if __name__ == "__main__":
    main()