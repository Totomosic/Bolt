#pragma once
#include "Module.h"

namespace Bolt::Assets
{

	class BLT_API RecoveryModule : public Module
	{
	public:
		int Run(int argc, const char** argv) override;
	};

}