#pragma once
#include "ResourceSettings.h"

namespace Bolt
{

	class BLT_API ResourceConverter
	{
	public:
		virtual ResourceSettings GetSettingsObject() const = 0;
		virtual bool Convert(const std::string& inputFile, const std::string& outputFile, ResourceSettings& settings) const = 0;
	};

}