#include "Texture2DConverter.h"

namespace Bolt
{

	Texture2DConverter::Texture2DConverter() : ResourceConverter()
	{
	}

	ResourceSettings Texture2DConverter::GetSettingsObject() const
	{
		ResourceSettings result(ResourceType::Texture2D);
		result.AddStringProperty("Name");
		result.AddIntProperty("Width", INT_MAX, 0, INT_MAX, false);
		result.AddIntProperty("Height", INT_MAX, 0, INT_MAX, false);
		result.AddIntProperty("Channels", 4, 1, 4, false);
		return result;
	}

	bool Texture2DConverter::Convert(const std::string& inputFile, const std::string& outputFile, ResourceSettings& settings) const
	{
		return false;
	}

}