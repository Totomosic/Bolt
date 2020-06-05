#include "ConverterModule.h"
#include "AssetsLib/AssetHeader.h"
#include "FileType.h"
#include "AssetsLib/filesystem.h"
#include "argparse.h"

#include "Converters/Texture2DConverter.h"

namespace Bolt::Assets
{

	static std::string CONVERTED_EXTENSION = ".bltasset";
	static std::string USE_FILE_EXTENSION = "";

	template<typename T>
	static T GetArgumentDefault(argparse::ArgumentParser& parser, const char* argumentName, const T& defaultValue)
	{
		if (parser.exists(argumentName))
		{
			return parser.get<T>(argumentName);
		}
		return defaultValue;
	}

	static FileType GetTypeFromExtension(const std::string& extension, const std::string& overrideString)
	{
		if (overrideString == USE_FILE_EXTENSION)
		{
			return GetFileTypeFromExtension(extension);
		}
		return GetFileTypeFromExtension("." + overrideString);
	}

	int ConverterModule::Run(int argc, const char** argv)
	{
		argparse::ArgumentParser parser("Converter Module");
		parser.enable_help();

		parser.add_argument("asset_filename", "The filename of the asset that is to be converted", true).position(argparse::ArgumentParser::Argument::Position::DONT_CARE);
		parser.add_argument("-o", "--output", "The output file to create", false);
		parser.add_argument("-t", "--type", "Override the asset type (needed when the extension doesn't match the asset type", false);
		parser.add_argument("-n", "--name", "The name of the asset", false);

		auto result = parser.parse(argc, argv);
		if (result)
		{
			BLT_CORE_FATAL(result.what());
			parser.print_help();
			return 1;
		}

		std::filesystem::path filename = parser.get<std::string>("asset_filename");
		std::filesystem::path outputFile = GetArgumentDefault<std::string>(parser, "output", (filename.parent_path() / filename.stem()).string() + CONVERTED_EXTENSION);
		std::string typeString = GetArgumentDefault<std::string>(parser, "type", USE_FILE_EXTENSION);
		std::string assetName = GetArgumentDefault<std::string>(parser, "name", filename.stem().string());
		FileType type = GetTypeFromExtension(filename.extension().string(), typeString);
		AssetType assetType = FileTypeToAssetType(type);

		if (type == FileType::Unknown || assetType == AssetType::Unknown)
		{
			BLT_CORE_FATAL("Invalid file type {}", (typeString == USE_FILE_EXTENSION) ? filename.extension().string() : ("." + typeString));
			return 1;
		}

		if (assetType == AssetType::Texture2D)
		{
			std::cout << "Converting " << filename << " as texture to " << outputFile << std::endl;
			return Texture2DConverter::ToBoltFormat(assetName, filename, outputFile, type);
		}
		return 1;
	}

}
