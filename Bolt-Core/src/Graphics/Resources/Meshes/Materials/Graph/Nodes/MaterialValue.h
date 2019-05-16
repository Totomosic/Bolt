#pragma once
#include "../../Shaders/Components/Values/ShaderValue.h"

namespace Bolt
{

	struct BLT_API MaterialValue
	{
	public:
		ShaderValuePtr Value;
		// Tries to compute value in Vertex -> Geometry -> Fragment (preference order)
		// Certain nodes (eg. SampleTexture node) will require to be calculated in the Fragment shader
		ShaderType MinimumShaderRequirement;
	};

}