#pragma once
#include "Values/__Values__.h"

namespace Bolt
{

	class BLT_API ShaderFuncs
	{
	public:
		ShaderFuncs() = delete;

		static ShaderFuncResultPtr Add(ShaderValuePtr left, ShaderValuePtr right);
		static ShaderFuncResultPtr Sub(ShaderValuePtr left, ShaderValuePtr right);
		static ShaderFuncResultPtr Mul(ShaderValuePtr left, ShaderValuePtr right);
		static ShaderFuncResultPtr Div(ShaderValuePtr left, ShaderValuePtr right);

	private:
		static ValueType DeduceOutputTypeNumeric(const ShaderValuePtr& left, const ShaderValuePtr& right);
		static bool TestDimensionEquality(const ShaderValuePtr& left, const ShaderValuePtr& right);
		static bool TestLegalMultiply(const ShaderValuePtr& left, const ShaderValuePtr& right);
		static bool TestNotMatrix(const ShaderValuePtr& left, const ShaderValuePtr& right);

	};

}