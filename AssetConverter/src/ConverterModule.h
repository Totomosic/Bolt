#pragma once
#include "Module.h"

namespace Bolt::Assets
{

	class BLT_API ConverterModule : public Module
	{
	public:
		int Run(int argc, const char** argv) override;
	};

}