#pragma once
#include "../ResourceConverter.h"

namespace Bolt
{

	class BLT_API Texture2DConverter : public ResourceConverter
	{
	public:
		Texture2DConverter();

		virtual ResourceSettings GetSettingsObject() const override;
		virtual bool Convert(const std::string& inputFile, const std::string& outputFile, ResourceSettings& settings) const override;
	};

}