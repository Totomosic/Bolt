#pragma once
#include "Logging.h"
#include <string>
#include "ResourceTypes.h"
#include "ResourceConverter.h"

namespace Bolt
{

	class BLT_API ConverterDispatcher
	{
	private:
		static std::unordered_map<ResourceType, std::unique_ptr<ResourceConverter>> s_Converters;

	private:
		std::string m_InputFile;
		std::string m_OutputFile;
		const std::string* m_SettingsFile;
		bool m_Write;

	public:
		ConverterDispatcher(const std::string& inputFile, const std::string& outputFile, const std::string* settingsFile, bool write);

		ResourceConverter& GetConverter() const;
		ResourceType GetType() const;

	public:
		template<typename T>
		static void RegisterConverter(ResourceType type)
		{
			s_Converters[type] = std::make_unique<T>();
		}


	private:
		ResourceType GetTypeFromSettingsFile(const std::string& settingsFile) const;
		ResourceType GetTypeFromFileExtension(const std::string& filename) const;

	};

}