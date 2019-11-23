#include "Logging.h"
#include <iostream>
#include <string>

#include "ConverterDispatcher.h"
#include "Converters/Texture2DConverter.h"

void usage(const char* programname)
{
	std::cout << "Usage: " << programname << " input_file [output_file] [[-w] settings_file]" << std::endl;
	exit(EXIT_FAILURE);
}

void ConvertResource(const std::string& inputFile, const std::string& outputFile, const std::string* settingsFile, bool write)
{
	Bolt::ConverterDispatcher dispatcher(inputFile, outputFile, settingsFile, write);
	if (dispatcher.GetType() != Bolt::ResourceType::None)
	{
		Bolt::ResourceConverter& converter = dispatcher.GetConverter();
		Bolt::ResourceSettings settings = converter.GetSettingsObject();
		if (settingsFile != nullptr && !write)
		{
			settings.PopulateFromFile(*settingsFile);
		}
		else
		{
			settings.PopulateFromStdin();
		}		
		if (!converter.Convert(inputFile, outputFile, settings))
		{
			BLT_ERROR("Failed to convert resource {0} -> {1}", inputFile, outputFile);
			exit(EXIT_FAILURE);
		}
		if (write && settingsFile != nullptr)
		{
			settings.WriteToFile(*settingsFile);
		}
	}
	else
	{
		BLT_ERROR("Unable to deduce resource type: {0}", inputFile);
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char** argv)
{
	Bolt::Log::Initialize();	
	if (argc < 2)
	{
		usage(argv[0]);
	}
	std::string inputFile = argv[1];
	size_t lastDot = inputFile.rfind('.');
	std::string outputFile = inputFile + ".bres";
	if (lastDot != std::string::npos)
	{
		outputFile = inputFile.substr(0, lastDot) + ".bres";
	}
	if (argc >= 3)
	{
		outputFile = argv[2];
	}
	std::string settingsFile;
	std::string* ptr = nullptr;
	bool write = false;
	if (argc >= 4)
	{
		if (argc == 5)
		{
			if (strcmp(argv[3], "-w") == 0)
			{
				write = true;
			}
			settingsFile = argv[4];
			ptr = &settingsFile;
		}
		else
		{
			settingsFile = argv[3];
			ptr = &settingsFile;
		}
	}

	Bolt::ConverterDispatcher::RegisterConverter<Bolt::Texture2DConverter>(Bolt::ResourceType::Texture2D);

	ConvertResource(inputFile, outputFile, ptr, write);
	return 0;
}