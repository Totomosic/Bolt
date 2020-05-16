#pragma once
#include "../Shaders/Components/Values/ShaderEnums.h"

namespace Bolt
{

	BLT_API enum class ShaderStageCompatibility
	{
		VertexOnly,
		GeometryOnly,
		FragmentOnly,
		VertexFragment,
		All
	};

	BLT_API inline bool IsShaderCompatible(ShaderStageCompatibility compatibility, ShaderStage stage)
	{
		return compatibility == ShaderStageCompatibility::All || (compatibility == ShaderStageCompatibility::VertexFragment && (stage == ShaderStage::Vertex || stage == ShaderStage::Fragment))
			|| (compatibility == ShaderStageCompatibility::VertexOnly && stage == ShaderStage::Vertex) || (compatibility == ShaderStageCompatibility::FragmentOnly && stage == ShaderStage::Fragment);
	}

	BLT_API inline bool IsShaderCompatible(ShaderStageCompatibility required, ShaderStageCompatibility compatibility)
	{
		if (compatibility == ShaderStageCompatibility::All)
		{
			return true;
		}
		if (required == ShaderStageCompatibility::All)
		{
			return false;
		}
		if ((required == ShaderStageCompatibility::VertexOnly || required == ShaderStageCompatibility::FragmentOnly) && compatibility == ShaderStageCompatibility::VertexFragment)
		{
			return true;
		}
		return required == compatibility;
	}

}