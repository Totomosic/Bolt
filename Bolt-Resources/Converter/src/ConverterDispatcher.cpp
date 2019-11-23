#include "ConverterDispatcher.h"

namespace Bolt
{

	std::unordered_map<ResourceType, std::unique_ptr<ResourceConverter>> ConverterDispatcher::s_Converters;

	ConverterDispatcher::ConverterDispatcher(const std::string& inputFile, const std::string& outputFile, const std::string* settingsFile, bool write)
		: m_InputFile(inputFile), m_OutputFile(outputFile), m_SettingsFile(settingsFile), m_Write(write)
	{
	}

	ResourceConverter& ConverterDispatcher::GetConverter() const
	{
		return *s_Converters[GetType()];
	}

	ResourceType ConverterDispatcher::GetType() const
	{
		if (m_SettingsFile != nullptr && !write)
		{
			return GetTypeFromSettingsFile(*m_SettingsFile);
		}
		return GetTypeFromFileExtension(m_InputFile);
	}

	ResourceType ConverterDispatcher::GetTypeFromSettingsFile(const std::string& settingsFile) const
	{
		return ResourceType::None;
	}

	ResourceType ConverterDispatcher::GetTypeFromFileExtension(const std::string& filename) const
	{
		size_t index = filename.rfind('.');
		if (index != std::string::npos)
		{
			std::string_view view(filename);
			std::string_view extension = view.substr(index + 1);
			if (extension == "jpg" || extension == "png" || extension == "jpeg")
			{
				return ResourceType::Texture2D;
			}
		}
		return ResourceType::None;
	}

}