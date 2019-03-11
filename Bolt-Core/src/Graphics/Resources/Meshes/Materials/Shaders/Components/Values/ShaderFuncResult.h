#pragma once
#include "ShaderValue.h"
#include "../BuiltInFunc.h"

namespace Bolt
{

	class ShaderFuncResult;
	using ShaderFuncResultPtr = std::shared_ptr<ShaderFuncResult>;

	class BLT_API ShaderFuncResult : public ShaderValue
	{
	private:
		std::vector<ShaderValuePtr> m_Inputs;
		BuiltInFunc m_Function;

	};

}