#pragma once
#include "XML\__XML__.h"

namespace Bolt
{

	class BLT_API FileLoader
	{
	public:
		static XMLnode LoadXML(File& xmlFile);
	};

}