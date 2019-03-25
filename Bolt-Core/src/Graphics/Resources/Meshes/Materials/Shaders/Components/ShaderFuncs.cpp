#include "Types.h"
#include "ShaderFuncs.h"

namespace Bolt
{

	ShaderFuncResultPtr ShaderFuncs::Add(ShaderValuePtr left, ShaderValuePtr right)
	{
		BLT_ASSERT(TestDimensionEquality(left, right), "Inputs must be of the same dimension to be added");
		ValueType resultType = DeduceOutputTypeNumeric(left, right);
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("(@0 + @1)", std::vector<ShaderValuePtr>{ std::move(left), std::move(right) }, resultType);
		return std::move(result);
	}

	ShaderFuncResultPtr ShaderFuncs::Sub(ShaderValuePtr left, ShaderValuePtr right)
	{
		BLT_ASSERT(TestDimensionEquality(left, right), "Inputs must be of the same dimension to be subtracted");
		ValueType resultType = DeduceOutputTypeNumeric(left, right);
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("(@0 - @1)", std::vector<ShaderValuePtr>{ std::move(left), std::move(right) }, resultType);
		return std::move(result);
	}

	ShaderFuncResultPtr ShaderFuncs::Mul(ShaderValuePtr left, ShaderValuePtr right)
	{
		BLT_ASSERT(TestDimensionEquality(left, right) || TestLegalMultiply(left, right), "Inputs must be of the same dimension to be multiplied");
		ValueType resultType = DeduceOutputTypeNumeric(left, right);
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("(@0 * @1)", std::vector<ShaderValuePtr>{ std::move(left), std::move(right) }, resultType);
		return std::move(result);
	}

	ShaderFuncResultPtr ShaderFuncs::Div(ShaderValuePtr left, ShaderValuePtr right)
	{
		BLT_ASSERT(TestDimensionEquality(left, right), "Inputs must be of the same dimension to be divided");
		BLT_ASSERT(TestNotMatrix(left, right), "Matrices cannot be divided");
		ValueType resultType = DeduceOutputTypeNumeric(left, right);
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("(@0 / @1)", std::vector<ShaderValuePtr>{ std::move(left), std::move(right) }, resultType);
		return std::move(result);
	}

	ValueType ShaderFuncs::DeduceOutputTypeNumeric(const ShaderValuePtr& left, const ShaderValuePtr& right)
	{
		if (ValueTypeIsMatrix(left->Type()) && ValueTypeIsMatrix(right->Type()))
		{
			return left->Type();
		}
		if (ValueTypeIsMatrix(left->Type()) && ValueTypeIsVector(right->Type()))
		{
			return right->Type();
		}
		if (left->Type() == ValueType::Vector4f || right->Type() == ValueType::Vector4f)
		{
			return ValueType::Vector4f;
		}
		if (left->Type() == ValueType::Vector3f || right->Type() == ValueType::Vector3f)
		{
			return ValueType::Vector3f;
		}
		if (left->Type() == ValueType::Vector2f || right->Type() == ValueType::Vector2f)
		{
			return ValueType::Vector2f;
		}
		if (left->Type() == ValueType::Float || right->Type() == ValueType::Float)
		{
			return ValueType::Float;
		}
		if (left->Type() == ValueType::Int || right->Type() == ValueType::Int)
		{
			return ValueType::Int;
		}
		BLT_ASSERT(false, "Unable to deduce type");
		return ValueType::Void;
	}

	bool ShaderFuncs::TestDimensionEquality(const ShaderValuePtr& left, const ShaderValuePtr& right)
	{
		if (ValueTypeIsVector(left->Type()))
		{
			return left->Type() == right->Type();
		}
		if (ValueTypeIsMatrix(left->Type()))
		{
			return left->Type() == right->Type();
		}
		return ValueTypeIsNumeric(left->Type()) && ValueTypeIsNumeric(right->Type());
	}

	bool ShaderFuncs::TestLegalMultiply(const ShaderValuePtr& left, const ShaderValuePtr& right)
	{
		if (ValueTypeIsNumeric(left->Type()) && ValueTypeIsNumeric(right->Type()))
		{
			return true;
		}
		if (ValueTypeIsVector(left->Type()) && ValueTypeIsNumeric(right->Type()))
		{
			return true;
		}
		if (ValueTypeIsNumeric(left->Type()) && ValueTypeIsVector(right->Type()))
		{
			return true;
		}
		if (ValueTypeIsVector(left->Type()) && ValueTypeIsVector(right->Type()))
		{
			return TestDimensionEquality(left, right);
		}
		if (ValueTypeIsMatrix(left->Type()) && ValueTypeIsMatrix(right->Type()))
		{
			return TestDimensionEquality(left, right);
		}
		if (ValueTypeIsMatrix(left->Type()) && ValueTypeIsVector(right->Type()))
		{
			if (left->Type() == ValueType::Matrix4f && right->Type() == ValueType::Vector4f)
			{
				return true;
			}
			if (left->Type() == ValueType::Matrix3f && right->Type() == ValueType::Vector3f)
			{
				return true;
			}
			if (left->Type() == ValueType::Matrix2f && right->Type() == ValueType::Vector2f)
			{
				return true;
			}
		}
		return false;
	}

	bool ShaderFuncs::TestNotMatrix(const ShaderValuePtr& left, const ShaderValuePtr& right)
	{
		return !(ValueTypeIsMatrix(left->Type()) || ValueTypeIsMatrix(right->Type()));
	}

}