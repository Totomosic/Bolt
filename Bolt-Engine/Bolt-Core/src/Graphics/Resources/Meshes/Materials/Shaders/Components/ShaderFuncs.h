#pragma once
#include "Values/ShaderFuncResult.h"
#include "Values/ShaderArrayValue.h"
#include "FunctionScope.h"

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
		static ShaderFuncResultPtr Pow(ShaderValuePtr left, ShaderValuePtr right);
		static ShaderFuncResultPtr Neg(ShaderValuePtr value);
		static ShaderFuncResultPtr Dot(ShaderValuePtr left, ShaderValuePtr right);
		static ShaderFuncResultPtr Cross(ShaderValuePtr left, ShaderValuePtr right);
		static ShaderFuncResultPtr Normalize(ShaderValuePtr value);
		static ShaderFuncResultPtr Length(ShaderValuePtr value);
		static ShaderFuncResultPtr Reflect(ShaderValuePtr vector, ShaderValuePtr normal);
		static ShaderFuncResultPtr Inverse(ShaderValuePtr value);
		static ShaderFuncResultPtr Transpose(ShaderValuePtr value);

		static ShaderFuncResultPtr LessThan(ShaderValuePtr left, ShaderValuePtr right);
		static ShaderFuncResultPtr LequalThan(ShaderValuePtr left, ShaderValuePtr right);
		static ShaderFuncResultPtr EqualTo(ShaderValuePtr left, ShaderValuePtr right);
		static ShaderFuncResultPtr GequalThan(ShaderValuePtr left, ShaderValuePtr right);
		static ShaderFuncResultPtr GreaterThan(ShaderValuePtr left, ShaderValuePtr right);
		static ShaderFuncResultPtr NotEqualTo(ShaderValuePtr left, ShaderValuePtr right);
		static ShaderFuncResultPtr Not(ShaderValuePtr value);

		static ShaderFuncResultPtr And(ShaderValuePtr left, ShaderValuePtr right);
		static ShaderFuncResultPtr Or(ShaderValuePtr left, ShaderValuePtr right);

		static ShaderFuncResultPtr Max(ShaderValuePtr left, ShaderValuePtr right);
		static ShaderFuncResultPtr Min(ShaderValuePtr left, ShaderValuePtr right);
		static ShaderFuncResultPtr Mix(ShaderValuePtr left, ShaderValuePtr right, ShaderValuePtr amount);

		static ShaderFuncResultPtr x(ShaderValuePtr value);
		static ShaderFuncResultPtr y(ShaderValuePtr value);
		static ShaderFuncResultPtr z(ShaderValuePtr value);
		static ShaderFuncResultPtr w(ShaderValuePtr value);
		static ShaderFuncResultPtr xy(ShaderValuePtr value);
		static ShaderFuncResultPtr yz(ShaderValuePtr value);
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
		
		static ShaderFuncResultPtr Matrix2(ShaderValuePtr r0, ShaderValuePtr r1);
		static ShaderFuncResultPtr Matrix3(ShaderValuePtr r0, ShaderValuePtr r1, ShaderValuePtr r2);
		static ShaderFuncResultPtr Matrix4(ShaderValuePtr r0, ShaderValuePtr r1, ShaderValuePtr r2, ShaderValuePtr r3);

		static ShaderFuncResultPtr SampleTexture(ShaderValuePtr texture, ShaderValuePtr texCoords);
		// Similar to SampleTexture but requires explicit lod - can be used in vertex shader
		static ShaderFuncResultPtr SampleTextureLod(ShaderValuePtr texture, ShaderValuePtr texCoords, ShaderValuePtr lod);

		static ShaderFuncResultPtr Call(const FunctionScope& func, const std::vector<ShaderValuePtr>& inputs);
		static ShaderArrayValuePtr Index(ShaderLValuePtr arr, ShaderValuePtr index);

	private:
		static ValueType DeduceOutputTypeNumeric(const ShaderValuePtr& left, const ShaderValuePtr& right);
		static bool TestDimensionEquality(const ShaderValuePtr& left, const ShaderValuePtr& right);
		static bool TestLegalMultiply(const ShaderValuePtr& left, const ShaderValuePtr& right);
		static bool TestNotMatrix(const ShaderValuePtr& left, const ShaderValuePtr& right);

	};

}