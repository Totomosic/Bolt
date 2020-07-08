#include "Texture2DConverter.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "AssetsLib/Image2D.h"
#include <fstream>

namespace Bolt::Assets
{

	int Texture2DConverter::ToBoltFormat(const std::string& assetName, const std::filesystem::path& filepath, const std::filesystem::path& output, FileType filetype)
	{
		if (!std::filesystem::exists(filepath))
		{
			BLT_CORE_ERROR("Texture file {} does not exist", filepath);
			return 1;
		}
		Image2D texture;
		int nComponents;
		stbi_set_flip_vertically_on_load(true);
		uint8_t* pixels = stbi_load(filepath.string().c_str(), &texture.Width, &texture.Height, &nComponents, 4);
		texture.Pixels = std::shared_ptr<uint8_t>(pixels, [](uint8_t* pixels)
			{
				stbi_image_free(pixels);
			});
		texture.Format = PixelFormat::RGBA;
		ByteStream stream = Texture2DEngine::CreateBoltFormat(assetName, texture);
		if (stream.GetLength() > 0)
		{
			if (!output.has_parent_path() || std::filesystem::create_directories(output.parent_path()))
			{
				std::ofstream file(output, std::ios::binary);
				file.write((const char*)stream.GetData(), stream.GetLength());
				file.close();
				return 0;
			}
			BLT_CORE_ERROR("Failed to create directories {}", output.parent_path());
		}
		return 1;
	}

	int Texture2DConverter::FromBoltFormat(const std::filesystem::path& filepath, const std::filesystem::path& output, FileType filetype)
	{
		if (!std::filesystem::exists(filepath))
		{
			BLT_CORE_ERROR("Texture file {} does not exist", filepath);
			return 1;
		}
		std::ifstream file(filepath, std::ios::in | std::ios::binary);
		size_t size = std::filesystem::file_size(filepath);
		char* data = new char[size];
		file.read(data, size);
		Asset<Image2D> texture = Texture2DEngine::ReadBoltFormat(data, size);
		delete[] data;
		file.close();

		stbi_flip_vertically_on_write(true);
		switch (filetype)
		{
		case FileType::PNG:
			stbi_write_png(output.string().c_str(), texture.Data.Width, texture.Data.Height, GetComponentCount(texture.Data.Format), texture.Data.Pixels.get(), texture.Data.Width * GetComponentCount(texture.Data.Format));
			break;
		case FileType::JPG:
			stbi_write_jpg(output.string().c_str(), texture.Data.Width, texture.Data.Height, GetComponentCount(texture.Data.Format), texture.Data.Pixels.get(), 90);
			break;
		case FileType::JPEG:
			stbi_write_jpg(output.string().c_str(), texture.Data.Width, texture.Data.Height, GetComponentCount(texture.Data.Format), texture.Data.Pixels.get(), 90);
			break;
		default:
			BLT_CORE_ERROR("Invalid file type");
			return 1;
		}
		
		return 0;
	}

}
