#pragma once
#include "../../Shaders/Components/Values/ShaderValue.h"

namespace Bolt
{

	struct BLT_API MaterialValue
	{
	public:
		ShaderValuePtr Value;
		ShaderStageCompatibility ShaderStage;
		ShaderType PreferredShaderStage;
	};

}