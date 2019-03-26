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

	ShaderFuncResultPtr ShaderFuncs::Neg(ShaderValuePtr value)
	{
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("-(@0)", std::vector<ShaderValuePtr>{ std::move(value) }, value->Type());
		return std::move(result);
	}

	ShaderFuncResultPtr ShaderFuncs::Dot(ShaderValuePtr left, ShaderValuePtr right)
	{
		BLT_ASSERT(TestDimensionEquality(left, right), "Inputs must be of the same dimension to be dotted");
		BLT_ASSERT(ValueTypeIsVector(left->Type()), "Inputs must be vectors to be dotted");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("dot(@0, @1)", std::vector<ShaderValuePtr>{ std::move(left), std::move(right) }, ValueType::Float);
		return std::move(result);
	}

	ShaderFuncResultPtr ShaderFuncs::Cross(ShaderValuePtr left, ShaderValuePtr right)
	{
		BLT_ASSERT(TestDimensionEquality(left, right), "Inputs must be of the same dimension to be dotted");
		BLT_ASSERT(left->Type() == ValueType::Vector3f, "Inputs must be Vec3 to be dotted");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("cross(@0, @1)", std::vector<ShaderValuePtr>{ std::move(left), std::move(right) }, ValueType::Vector3f);
		return std::move(result);
	}

	ShaderFuncResultPtr ShaderFuncs::x(ShaderValuePtr value)
	{
		BLT_ASSERT(ValueTypeIsVector(value->Type()), "Inputs must be vectors to be swizzelled");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("(@0).x", std::vector<ShaderValuePtr>{ std::move(value) }, ValueType::Float);
		return std::move(result);
	}

	ShaderFuncResultPtr ShaderFuncs::y(ShaderValuePtr value)
	{
		BLT_ASSERT(ValueTypeIsVector(value->Type()), "Inputs must be vectors to be swizzelled");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("(@0).y", std::vector<ShaderValuePtr>{ std::move(value) }, ValueType::Float);
		return std::move(result);
	}

	ShaderFuncResultPtr ShaderFuncs::z(ShaderValuePtr value)
	{
		BLT_ASSERT(ValueTypeIsVector(value->Type()), "Inputs must be vectors to be swizzelled");
		BLT_ASSERT(ValueTypeGetDimension(value->Type()) > 2, "Input must be greater than 2D");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("(@0).z", std::vector<ShaderValuePtr>{ std::move(value) }, ValueType::Float);
		return std::move(result);
	}

	ShaderFuncResultPtr ShaderFuncs::xy(ShaderValuePtr value)
	{
		BLT_ASSERT(ValueTypeIsVector(value->Type()), "Inputs must be vectors to be swizzelled");
		BLT_ASSERT(ValueTypeGetDimension(value->Type()) > 2, "Input must be greater than 2D");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("(@0).xy", std::vector<ShaderValuePtr>{ std::move(value) }, ValueType::Vector2f);
		return std::move(result);
	}

	ShaderFuncResultPtr ShaderFuncs::xz(ShaderValuePtr value)
	{
		BLT_ASSERT(ValueTypeIsVector(value->Type()), "Inputs must be vectors to be swizzelled");
		BLT_ASSERT(ValueTypeGetDimension(value->Type()) > 2, "Input must be greater than 2D");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("(@0).xz", std::vector<ShaderValuePtr>{ std::move(value) }, ValueType::Vector2f);
		return std::move(result);
	}

	ShaderFuncResultPtr ShaderFuncs::xyz(ShaderValuePtr value)
	{
		BLT_ASSERT(ValueTypeIsVector(value->Type()), "Inputs must be vectors to be swizzelled");
		BLT_ASSERT(ValueTypeGetDimension(value->Type()) > 3, "Input must be greater than 3D");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("(@0).xyz", std::vector<ShaderValuePtr>{ std::move(value) }, ValueType::Vector3f);
		return std::move(result);
	}

	ShaderFuncResultPtr ShaderFuncs::Vec2(ShaderValuePtr x, ShaderValuePtr y)
	{
		BLT_ASSERT(ValueTypeIsNumeric(x->Type()) && ValueTypeIsNumeric(y->Type()), "x and y should be numbers");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("vec2(@0, @1)", std::vector<ShaderValuePtr>{ std::move(x), std::move(y) }, ValueType::Vector2f);
		return std::move(result);
	}

	ShaderFuncResultPtr ShaderFuncs::Vec2(ShaderValuePtr xy)
	{
		BLT_ASSERT(ValueTypeIsNumeric(xy->Type()), "xy should be a number");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("vec2(@0)", std::vector<ShaderValuePtr>{ std::move(xy) }, ValueType::Vector2f);
		return std::move(result);
	}

	ShaderFuncResultPtr ShaderFuncs::Vec3(ShaderValuePtr x, ShaderValuePtr y, ShaderValuePtr z)
	{
		BLT_ASSERT(ValueTypeIsNumeric(x->Type()) && ValueTypeIsNumeric(y->Type()) && ValueTypeIsNumeric(z->Type()), "x, y and z should be numbers");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("vec3(@0, @1, @2)", std::vector<ShaderValuePtr>{ std::move(x), std::move(y), std::move(z) }, ValueType::Vector3f);
		return std::move(result);
	}

	ShaderFuncResultPtr ShaderFuncs::Vec3(ShaderValuePtr xy, ShaderValuePtr z)
	{
		BLT_ASSERT(ValueTypeIsNumeric(z->Type()), "z should be numbers");
		BLT_ASSERT(xy->Type() == ValueType::Vector2f, "xy should be a vec2");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("vec3(@0, @1)", std::vector<ShaderValuePtr>{ std::move(xy), std::move(z) }, ValueType::Vector3f);
		return std::move(result);
	}

	ShaderFuncResultPtr ShaderFuncs::Vec3(ShaderValuePtr xyz)
	{
		BLT_ASSERT(ValueTypeIsNumeric(xyz->Type()), "xyz should be a number");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("vec3(@0)", std::vector<ShaderValuePtr>{ std::move(xyz) }, ValueType::Vector3f);
		return std::move(result);
	}

	ShaderFuncResultPtr ShaderFuncs::Vec4(ShaderValuePtr x, ShaderValuePtr y, ShaderValuePtr z, ShaderValuePtr w)
	{
		BLT_ASSERT(ValueTypeIsNumeric(x->Type()) && ValueTypeIsNumeric(y->Type()) && ValueTypeIsNumeric(z->Type()) && ValueTypeIsNumeric(w->Type()), "x, y, z and w should be numbers");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("vec4(@0, @1, @2, @3)", std::vector<ShaderValuePtr>{ std::move(x), std::move(y), std::move(z), std::move(w) }, ValueType::Vector4f);
		return std::move(result);
	}

	ShaderFuncResultPtr ShaderFuncs::Vec4(ShaderValuePtr xy, ShaderValuePtr z, ShaderValuePtr w)
	{
		BLT_ASSERT(ValueTypeIsNumeric(z->Type()) && ValueTypeIsNumeric(w->Type()), "z and w should be numbers");
		BLT_ASSERT(xy->Type() == ValueType::Vector2f, "xy should be a vec2");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("vec4(@0, @1, @2)", std::vector<ShaderValuePtr>{ std::move(xy), std::move(z), std::move(w) }, ValueType::Vector4f);
		return std::move(result);
	}

	ShaderFuncResultPtr ShaderFuncs::Vec4(ShaderValuePtr xyz, ShaderValuePtr w)
	{
		BLT_ASSERT(ValueTypeIsNumeric(w->Type()), "w should be a number");
		BLT_ASSERT(xyz->Type() == ValueType::Vector3f, "xyz should be a vec3");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("vec4(@0, @1)", std::vector<ShaderValuePtr>{ std::move(xyz), std::move(w) }, ValueType::Vector4f);
		return std::move(result);
	}

	ShaderFuncResultPtr ShaderFuncs::Vec4(ShaderValuePtr xyzw)
	{
		BLT_ASSERT(ValueTypeIsNumeric(xyzw->Type()), "xyzw should be a number");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("vec4(@0)", std::vector<ShaderValuePtr>{ std::move(xyzw) }, ValueType::Vector4f);
		return std::move(result);
	}

	ShaderFuncResultPtr ShaderFuncs::SampleTexture(ShaderValuePtr texture, ShaderValuePtr texCoords)
	{
		BLT_ASSERT(texture->Type() == ValueType::Texture2D && texCoords->Type() == ValueType::Vector2f, "Only 2D textures supported");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("texture(@0, @1)", std::vector<ShaderValuePtr>{ std::move(texture), std::move(texCoords) }, ValueType::Vector4f);
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