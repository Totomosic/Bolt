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
		static ShaderFuncResultPtr Neg(ShaderValuePtr value);
		static ShaderFuncResultPtr Dot(ShaderValuePtr left, ShaderValuePtr right);
		static ShaderFuncResultPtr Cross(ShaderValuePtr left, ShaderValuePtr right);

		static ShaderFuncResultPtr x(ShaderValuePtr value);
		static ShaderFuncResultPtr y(ShaderValuePtr value);
		static ShaderFuncResultPtr z(ShaderValuePtr value);
		static ShaderFuncResultPtr xy(ShaderValuePtr value);
		static ShaderFuncResultPtr xz(ShaderValuePtr value);
		static ShaderFuncResultPtr xyz(ShaderValuePtr value);

		static ShaderFuncResultPtr Vec2(ShaderValuePtr x, ShaderValuePtr y);
		static ShaderFuncResultPtr Vec2(ShaderValuePtr xy);
		static ShaderFuncResultPtr Vec3(ShaderValuePtr x, ShaderValuePtr y, ShaderValuePtr z);
		static ShaderFuncResultPtr Vec3(ShaderValuePtr xy, ShaderValuePtr z);
		static ShaderFuncResultPtr Vec3(ShaderValuePtr xyz);
		static ShaderFuncResultPtr Vec4(ShaderValuePtr x, ShaderValuePtr y, ShaderValuePtr z, ShaderValuePtr w);
		static ShaderFuncResultPtr Vec4(ShaderValuePtr xy, ShaderValuePtr z, ShaderValuePtr w);
		static ShaderFuncResultPtr Vec4(ShaderValuePtr xyz, ShaderValuePtr w);
		static ShaderFuncResultPtr Vec4(ShaderValuePtr xwyz);

		static ShaderFuncResultPtr SampleTexture(ShaderValuePtr texture, ShaderValuePtr texCoords);

	private:
		static ValueType DeduceOutputTypeNumeric(const ShaderValuePtr& left, const ShaderValuePtr& right);
		static bool TestDimensionEquality(const ShaderValuePtr& left, const ShaderValuePtr& right);
		static bool TestLegalMultiply(const ShaderValuePtr& left, const ShaderValuePtr& right);
		static bool TestNotMatrix(const ShaderValuePtr& left, const ShaderValuePtr& right);

	};

}