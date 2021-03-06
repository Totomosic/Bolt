#include "bltpch.h"
#include "ShaderFuncs.h"

namespace Bolt
{

	ShaderFuncResultPtr ShaderFuncs::Add(ShaderValuePtr left, ShaderValuePtr right)
	{
		BLT_ASSERT(left->TypeDimension() == ValueTypeDim::Single && right->TypeDimension() == ValueTypeDim::Single, "Cannot operate on arrays");
		ValueType resultType = DeduceOutputTypeNumeric(left, right);
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("(@0+@1)", std::vector<ShaderValuePtr>{ std::move(left), std::move(right) }, resultType);
		return result;
	}

	ShaderFuncResultPtr ShaderFuncs::Sub(ShaderValuePtr left, ShaderValuePtr right)
	{
		BLT_ASSERT(left->TypeDimension() == ValueTypeDim::Single && right->TypeDimension() == ValueTypeDim::Single, "Cannot operate on arrays");
		ValueType resultType = DeduceOutputTypeNumeric(left, right);
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("(@0-@1)", std::vector<ShaderValuePtr>{ std::move(left), std::move(right) }, resultType);
		return result;
	}

	ShaderFuncResultPtr ShaderFuncs::Mul(ShaderValuePtr left, ShaderValuePtr right)
	{
		BLT_ASSERT(left->TypeDimension() == ValueTypeDim::Single && right->TypeDimension() == ValueTypeDim::Single, "Cannot operate on arrays");
		BLT_ASSERT(TestDimensionEquality(left, right) || TestLegalMultiply(left, right), "Inputs must be of the same dimension to be multiplied");
		ValueType resultType = DeduceOutputTypeNumeric(left, right);
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("(@0*@1)", std::vector<ShaderValuePtr>{ std::move(left), std::move(right) }, resultType);
		return result;
	}

	ShaderFuncResultPtr ShaderFuncs::Div(ShaderValuePtr left, ShaderValuePtr right)
	{
		BLT_ASSERT(left->TypeDimension() == ValueTypeDim::Single && right->TypeDimension() == ValueTypeDim::Single, "Cannot operate on arrays");
		BLT_ASSERT(TestNotMatrix(left, right), "Matrices cannot be divided");
		ValueType resultType = DeduceOutputTypeNumeric(left, right);
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("(@0/@1)", std::vector<ShaderValuePtr>{ std::move(left), std::move(right) }, resultType);
		return result;
	}

	ShaderFuncResultPtr ShaderFuncs::Pow(ShaderValuePtr left, ShaderValuePtr right)
	{
		BLT_ASSERT(left->TypeDimension() == ValueTypeDim::Single && right->TypeDimension() == ValueTypeDim::Single, "Cannot operate on arrays");
		BLT_ASSERT(left->Type() == right->Type(), "Inputs must be of same type");
		BLT_ASSERT(TestNotMatrix(left, right), "Matrices cannot be pow");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("pow(@0,@1)", std::vector<ShaderValuePtr>{ std::move(left), std::move(right) }, left->Type());
		return result;
	}

	ShaderFuncResultPtr ShaderFuncs::Neg(ShaderValuePtr value)
	{
		BLT_ASSERT(value->TypeDimension() == ValueTypeDim::Single, "Cannot operate on arrays");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("-(@0)", std::vector<ShaderValuePtr>{ std::move(value) }, value->Type());
		return result;
	}

	ShaderFuncResultPtr ShaderFuncs::Dot(ShaderValuePtr left, ShaderValuePtr right)
	{
		BLT_ASSERT(left->TypeDimension() == ValueTypeDim::Single && right->TypeDimension() == ValueTypeDim::Single, "Cannot operate on arrays");
		BLT_ASSERT(TestDimensionEquality(left, right), "Inputs must be of the same dimension to be dotted");
		BLT_ASSERT(ValueTypeIsVector(left->Type()), "Inputs must be vectors to be dotted");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("dot(@0,@1)", std::vector<ShaderValuePtr>{ std::move(left), std::move(right) }, ValueType::Float);
		return result;
	}

	ShaderFuncResultPtr ShaderFuncs::Cross(ShaderValuePtr left, ShaderValuePtr right)
	{
		BLT_ASSERT(left->TypeDimension() == ValueTypeDim::Single && right->TypeDimension() == ValueTypeDim::Single, "Cannot operate on arrays");
		BLT_ASSERT(TestDimensionEquality(left, right), "Inputs must be of the same dimension to be dotted");
		BLT_ASSERT(left->Type() == ValueType::Vector3f, "Inputs must be Vec3 to be crossed");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("cross(@0,@1)", std::vector<ShaderValuePtr>{ std::move(left), std::move(right) }, ValueType::Vector3f);
		return result;
	}

	ShaderFuncResultPtr ShaderFuncs::Normalize(ShaderValuePtr value)
	{
		BLT_ASSERT(value->TypeDimension() == ValueTypeDim::Single, "Cannot operate on arrays");
		BLT_ASSERT(ValueTypeIsVector(value->Type()), "Can only normalize vectors");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("normalize(@0)", std::vector<ShaderValuePtr>{ std::move(value) }, value->Type());
		return result;
	}

	ShaderFuncResultPtr ShaderFuncs::Length(ShaderValuePtr value)
	{
		BLT_ASSERT(value->TypeDimension() == ValueTypeDim::Single, "Cannot operate on arrays");
		BLT_ASSERT(ValueTypeIsVector(value->Type()), "Can only find length of vectors");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("length(@0)", std::vector<ShaderValuePtr>{ std::move(value) }, ValueType::Float);
		return result;
	}

	ShaderFuncResultPtr ShaderFuncs::Reflect(ShaderValuePtr vector, ShaderValuePtr normal)
	{
		BLT_ASSERT(vector->TypeDimension() == ValueTypeDim::Single && normal->TypeDimension() == ValueTypeDim::Single, "Cannot operate on arrays");
		BLT_ASSERT(vector->Type() == ValueType::Vector3f && normal->Type() == ValueType::Vector3f, "Inputs must be Vec3 to be reflected");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("reflect(@0,@1)", std::vector<ShaderValuePtr>{ std::move(vector), std::move(normal) }, ValueType::Vector3f);
		return result;
	}

	ShaderFuncResultPtr ShaderFuncs::Inverse(ShaderValuePtr value)
	{
		BLT_ASSERT(ValueTypeIsMatrix(value->Type()), "Can only inverse matrices");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("inverse(@0)", std::vector<ShaderValuePtr>{ std::move(value) }, value->Type());
		return result;
	}

	ShaderFuncResultPtr ShaderFuncs::Transpose(ShaderValuePtr value)
	{
		BLT_ASSERT(ValueTypeIsMatrix(value->Type()), "Can only transpose matrices");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("transpose(@0)", std::vector<ShaderValuePtr>{ std::move(value) }, value->Type());
		return result;
	}

	ShaderFuncResultPtr ShaderFuncs::LessThan(ShaderValuePtr left, ShaderValuePtr right)
	{
		BLT_ASSERT(left->TypeDimension() == ValueTypeDim::Single && right->TypeDimension() == ValueTypeDim::Single, "Cannot operate on arrays");
		BLT_ASSERT(ValueTypeIsNumeric(left->Type()) && ValueTypeIsNumeric(right->Type()), "Inputs must be numeric to be compared");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("(@0<@1)", std::vector<ShaderValuePtr>{ std::move(left), std::move(right) }, ValueType::Bool);
		return result;
	}

	ShaderFuncResultPtr ShaderFuncs::LequalThan(ShaderValuePtr left, ShaderValuePtr right)
	{
		BLT_ASSERT(left->TypeDimension() == ValueTypeDim::Single && right->TypeDimension() == ValueTypeDim::Single, "Cannot operate on arrays");
		BLT_ASSERT(ValueTypeIsNumeric(left->Type()) && ValueTypeIsNumeric(right->Type()), "Inputs must be numeric to be compared");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("(@0<=@1)", std::vector<ShaderValuePtr>{ std::move(left), std::move(right) }, ValueType::Bool);
		return result;
	}

	ShaderFuncResultPtr ShaderFuncs::EqualTo(ShaderValuePtr left, ShaderValuePtr right)
	{
		BLT_ASSERT(left->TypeDimension() == ValueTypeDim::Single && right->TypeDimension() == ValueTypeDim::Single, "Cannot operate on arrays");
		BLT_ASSERT(ValueTypeIsNumeric(left->Type()) && ValueTypeIsNumeric(right->Type()), "Inputs must be numeric to be compared");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("(@0==@1)", std::vector<ShaderValuePtr>{ std::move(left), std::move(right) }, ValueType::Bool);
		return result;
	}

	ShaderFuncResultPtr ShaderFuncs::GequalThan(ShaderValuePtr left, ShaderValuePtr right)
	{
		BLT_ASSERT(left->TypeDimension() == ValueTypeDim::Single && right->TypeDimension() == ValueTypeDim::Single, "Cannot operate on arrays");
		BLT_ASSERT(ValueTypeIsNumeric(left->Type()) && ValueTypeIsNumeric(right->Type()), "Inputs must be numeric to be compared");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("(@0>=@1)", std::vector<ShaderValuePtr>{ std::move(left), std::move(right) }, ValueType::Bool);
		return result;
	}

	ShaderFuncResultPtr ShaderFuncs::GreaterThan(ShaderValuePtr left, ShaderValuePtr right)
	{
		BLT_ASSERT(left->TypeDimension() == ValueTypeDim::Single && right->TypeDimension() == ValueTypeDim::Single, "Cannot operate on arrays");
		BLT_ASSERT(ValueTypeIsNumeric(left->Type()) && ValueTypeIsNumeric(right->Type()), "Inputs must be numeric to be compared");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("(@0>@1)", std::vector<ShaderValuePtr>{ std::move(left), std::move(right) }, ValueType::Bool);
		return result;
	}

	ShaderFuncResultPtr ShaderFuncs::NotEqualTo(ShaderValuePtr left, ShaderValuePtr right)
	{
		BLT_ASSERT(left->TypeDimension() == ValueTypeDim::Single && right->TypeDimension() == ValueTypeDim::Single, "Cannot operate on arrays");
		BLT_ASSERT(TestDimensionEquality(left, right), "Inputs must be same dimension");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("(@0!=@1)", std::vector<ShaderValuePtr>{ std::move(left), std::move(right) }, ValueType::Bool);
		return result;
	}

	ShaderFuncResultPtr ShaderFuncs::Not(ShaderValuePtr value)
	{
		BLT_ASSERT(value->TypeDimension() == ValueTypeDim::Single, "Cannot operate on arrays");
		BLT_ASSERT(value->Type() == ValueType::Bool, "Input must be boolean");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("!(@0)", std::vector<ShaderValuePtr>{ std::move(value) }, ValueType::Bool);
		return result;
	}

	ShaderFuncResultPtr ShaderFuncs::And(ShaderValuePtr left, ShaderValuePtr right)
	{
		BLT_ASSERT(left->TypeDimension() == ValueTypeDim::Single && right->TypeDimension() == ValueTypeDim::Single, "Cannot operate on arrays");
		BLT_ASSERT(left->Type() == ValueType::Bool && right->Type() == ValueType::Bool, "Input must be boolean");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("((@0)&&(@1))", std::vector<ShaderValuePtr>{ std::move(left), std::move(right) }, ValueType::Bool);
		return result;
	}

	ShaderFuncResultPtr ShaderFuncs::Or(ShaderValuePtr left, ShaderValuePtr right)
	{
		BLT_ASSERT(left->TypeDimension() == ValueTypeDim::Single && right->TypeDimension() == ValueTypeDim::Single, "Cannot operate on arrays");
		BLT_ASSERT(left->Type() == ValueType::Bool && right->Type() == ValueType::Bool, "Input must be boolean");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("((@0)||(@1))", std::vector<ShaderValuePtr>{ std::move(left), std::move(right) }, ValueType::Bool);
		return result;
	}

	ShaderFuncResultPtr ShaderFuncs::Max(ShaderValuePtr left, ShaderValuePtr right)
	{
		BLT_ASSERT(left->TypeDimension() == ValueTypeDim::Single && right->TypeDimension() == ValueTypeDim::Single, "Cannot operate on arrays");
		BLT_ASSERT(ValueTypeIsNumeric(left->Type()), "Input must be numeric");
		BLT_ASSERT(left->Type() == right->Type(), "Types must match");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("max(@0,@1)", std::vector<ShaderValuePtr>{ std::move(left), std::move(right) }, left->Type());
		return result;
	}

	ShaderFuncResultPtr ShaderFuncs::Min(ShaderValuePtr left, ShaderValuePtr right)
	{
		BLT_ASSERT(left->TypeDimension() == ValueTypeDim::Single && right->TypeDimension() == ValueTypeDim::Single, "Cannot operate on arrays");;
		BLT_ASSERT(ValueTypeIsNumeric(left->Type()), "Input must be numeric");
		BLT_ASSERT(left->Type() == right->Type(), "Types must match");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("min(@0,@1)", std::vector<ShaderValuePtr>{ std::move(left), std::move(right) }, left->Type());
		return result;
	}

	ShaderFuncResultPtr ShaderFuncs::Mix(ShaderValuePtr left, ShaderValuePtr right, ShaderValuePtr amount)
	{
		BLT_ASSERT(left->TypeDimension() == ValueTypeDim::Single && right->TypeDimension() == ValueTypeDim::Single, "Cannot operate on arrays");;
		BLT_ASSERT(left->Type() == right->Type(), "Types must match");
		BLT_ASSERT(amount->Type() == ValueType::Float, "Amount must be float")
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("mix(@0,@1,@2)", std::vector<ShaderValuePtr>{ std::move(left), std::move(right), std::move(amount) }, left->Type());
		return result;
	}

	ShaderFuncResultPtr ShaderFuncs::x(ShaderValuePtr value)
	{
		BLT_ASSERT(value->TypeDimension() == ValueTypeDim::Single, "Cannot operate on arrays");
		BLT_ASSERT(ValueTypeIsVector(value->Type()), "Inputs must be vectors to be swizzelled");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("(@0).x", std::vector<ShaderValuePtr>{ std::move(value) }, ValueType::Float);
		return result;
	}

	ShaderFuncResultPtr ShaderFuncs::y(ShaderValuePtr value)
	{
		BLT_ASSERT(value->TypeDimension() == ValueTypeDim::Single, "Cannot operate on arrays");
		BLT_ASSERT(ValueTypeIsVector(value->Type()), "Inputs must be vectors to be swizzelled");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("(@0).y", std::vector<ShaderValuePtr>{ std::move(value) }, ValueType::Float);
		return result;
	}

	ShaderFuncResultPtr ShaderFuncs::z(ShaderValuePtr value)
	{
		BLT_ASSERT(value->TypeDimension() == ValueTypeDim::Single, "Cannot operate on arrays");
		BLT_ASSERT(ValueTypeIsVector(value->Type()), "Inputs must be vectors to be swizzelled");
		BLT_ASSERT(ValueTypeGetDimension(value->Type()) > 2, "Input must be greater than 2D");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("(@0).z", std::vector<ShaderValuePtr>{ std::move(value) }, ValueType::Float);
		return result;
	}

	ShaderFuncResultPtr ShaderFuncs::w(ShaderValuePtr value)
	{
		BLT_ASSERT(value->TypeDimension() == ValueTypeDim::Single, "Cannot operate on arrays");
		BLT_ASSERT(ValueTypeIsVector(value->Type()), "Inputs must be vectors to be swizzelled");
		BLT_ASSERT(ValueTypeGetDimension(value->Type()) > 3, "Input must be greater than 3D");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("(@0).w", std::vector<ShaderValuePtr>{ std::move(value) }, ValueType::Float);
		return result;
	}

	ShaderFuncResultPtr ShaderFuncs::xy(ShaderValuePtr value)
	{
		BLT_ASSERT(value->TypeDimension() == ValueTypeDim::Single, "Cannot operate on arrays");
		BLT_ASSERT(ValueTypeIsVector(value->Type()), "Inputs must be vectors to be swizzelled");
		BLT_ASSERT(ValueTypeGetDimension(value->Type()) > 2, "Input must be greater than 2D");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("(@0).xy", std::vector<ShaderValuePtr>{ std::move(value) }, ValueType::Vector2f);
		return result;
	}

	ShaderFuncResultPtr ShaderFuncs::yz(ShaderValuePtr value)
	{
		BLT_ASSERT(value->TypeDimension() == ValueTypeDim::Single, "Cannot operate on arrays");
		BLT_ASSERT(ValueTypeIsVector(value->Type()), "Inputs must be vectors to be swizzelled");
		BLT_ASSERT(ValueTypeGetDimension(value->Type()) > 2, "Input must be greater than 2D");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("(@0).yz", std::vector<ShaderValuePtr>{ std::move(value) }, ValueType::Vector2f);
		return result;
	}

	ShaderFuncResultPtr ShaderFuncs::xz(ShaderValuePtr value)
	{
		BLT_ASSERT(value->TypeDimension() == ValueTypeDim::Single, "Cannot operate on arrays");
		BLT_ASSERT(ValueTypeIsVector(value->Type()), "Inputs must be vectors to be swizzelled");
		BLT_ASSERT(ValueTypeGetDimension(value->Type()) > 2, "Input must be greater than 2D");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("(@0).xz", std::vector<ShaderValuePtr>{ std::move(value) }, ValueType::Vector2f);
		return result;
	}

	ShaderFuncResultPtr ShaderFuncs::xyz(ShaderValuePtr value)
	{
		BLT_ASSERT(value->TypeDimension() == ValueTypeDim::Single, "Cannot operate on arrays");
		BLT_ASSERT(ValueTypeIsVector(value->Type()), "Inputs must be vectors to be swizzelled");
		BLT_ASSERT(ValueTypeGetDimension(value->Type()) > 3, "Input must be greater than 3D");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("(@0).xyz", std::vector<ShaderValuePtr>{ std::move(value) }, ValueType::Vector3f);
		return result;
	}

	ShaderFuncResultPtr ShaderFuncs::Vec2(ShaderValuePtr x, ShaderValuePtr y)
	{
		BLT_ASSERT(ValueTypeIsNumeric(x->Type()) && ValueTypeIsNumeric(y->Type()), "x and y should be numbers");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("vec2(@0,@1)", std::vector<ShaderValuePtr>{ std::move(x), std::move(y) }, ValueType::Vector2f);
		return result;
	}

	ShaderFuncResultPtr ShaderFuncs::Vec2(ShaderValuePtr xy)
	{
		BLT_ASSERT(ValueTypeIsNumeric(xy->Type()), "xy should be a number");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("vec2(@0)", std::vector<ShaderValuePtr>{ std::move(xy) }, ValueType::Vector2f);
		return result;
	}

	ShaderFuncResultPtr ShaderFuncs::Vec3(ShaderValuePtr x, ShaderValuePtr y, ShaderValuePtr z)
	{
		BLT_ASSERT(ValueTypeIsNumeric(x->Type()) && ValueTypeIsNumeric(y->Type()) && ValueTypeIsNumeric(z->Type()), "x, y and z should be numbers");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("vec3(@0,@1,@2)", std::vector<ShaderValuePtr>{ std::move(x), std::move(y), std::move(z) }, ValueType::Vector3f);
		return result;
	}

	ShaderFuncResultPtr ShaderFuncs::Vec3(ShaderValuePtr xy, ShaderValuePtr z)
	{
		BLT_ASSERT(ValueTypeIsNumeric(z->Type()), "z should be numbers");
		BLT_ASSERT(xy->Type() == ValueType::Vector2f, "xy should be a vec2");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("vec3(@0,@1)", std::vector<ShaderValuePtr>{ std::move(xy), std::move(z) }, ValueType::Vector3f);
		return result;
	}

	ShaderFuncResultPtr ShaderFuncs::Vec3(ShaderValuePtr xyz)
	{
		BLT_ASSERT(ValueTypeIsNumeric(xyz->Type()), "xyz should be a number");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("vec3(@0)", std::vector<ShaderValuePtr>{ std::move(xyz) }, ValueType::Vector3f);
		return result;
	}

	ShaderFuncResultPtr ShaderFuncs::Vec4(ShaderValuePtr x, ShaderValuePtr y, ShaderValuePtr z, ShaderValuePtr w)
	{
		BLT_ASSERT(ValueTypeIsNumeric(x->Type()) && ValueTypeIsNumeric(y->Type()) && ValueTypeIsNumeric(z->Type()) && ValueTypeIsNumeric(w->Type()), "x, y, z and w should be numbers");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("vec4(@0,@1,@2,@3)", std::vector<ShaderValuePtr>{ std::move(x), std::move(y), std::move(z), std::move(w) }, ValueType::Vector4f);
		return result;
	}

	ShaderFuncResultPtr ShaderFuncs::Vec4(ShaderValuePtr xy, ShaderValuePtr z, ShaderValuePtr w)
	{
		BLT_ASSERT(ValueTypeIsNumeric(z->Type()) && ValueTypeIsNumeric(w->Type()), "z and w should be numbers");
		BLT_ASSERT(xy->Type() == ValueType::Vector2f, "xy should be a vec2");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("vec4(@0,@1,@2)", std::vector<ShaderValuePtr>{ std::move(xy), std::move(z), std::move(w) }, ValueType::Vector4f);
		return result;
	}

	ShaderFuncResultPtr ShaderFuncs::Vec4(ShaderValuePtr xyz, ShaderValuePtr w)
	{
		BLT_ASSERT(ValueTypeIsNumeric(w->Type()), "w should be a number");
		BLT_ASSERT(xyz->Type() == ValueType::Vector3f, "xyz should be a vec3");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("vec4(@0,@1)", std::vector<ShaderValuePtr>{ std::move(xyz), std::move(w) }, ValueType::Vector4f);
		return result;
	}

	ShaderFuncResultPtr ShaderFuncs::Vec4(ShaderValuePtr xyzw)
	{
		BLT_ASSERT(ValueTypeIsNumeric(xyzw->Type()), "xyzw should be a number");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("vec4(@0)", std::vector<ShaderValuePtr>{ std::move(xyzw) }, ValueType::Vector4f);
		return result;
	}

	ShaderFuncResultPtr ShaderFuncs::Matrix2(ShaderValuePtr r0, ShaderValuePtr r1)
	{
		BLT_ASSERT(r0->Type() == ValueType::Vector2f && r1->Type() == ValueType::Vector2f, "Rows must be Vector2f");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("mat2(@0,@1)", std::vector<ShaderValuePtr>{ std::move(r0), std::move(r1) }, ValueType::Matrix2f);
		return result;
	}

	ShaderFuncResultPtr ShaderFuncs::Matrix3(ShaderValuePtr r0, ShaderValuePtr r1, ShaderValuePtr r2)
	{
		BLT_ASSERT(r0->Type() == ValueType::Vector3f && r1->Type() == ValueType::Vector3f && r2->Type() == ValueType::Vector3f, "Rows must be Vector3f");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("mat3(@0,@1,@2)", std::vector<ShaderValuePtr>{ std::move(r0), std::move(r1), std::move(r2) }, ValueType::Matrix3f);
		return result;
	}

	ShaderFuncResultPtr ShaderFuncs::Matrix3(ShaderValuePtr mat4)
	{
		BLT_ASSERT(mat4->Type() == ValueType::Matrix4f, "Arg must be a Matrix4f");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("mat3(@0)", std::vector<ShaderValuePtr>{ std::move(mat4) }, ValueType::Matrix3f);
		return result;
	}

	ShaderFuncResultPtr ShaderFuncs::Matrix4(ShaderValuePtr r0, ShaderValuePtr r1, ShaderValuePtr r2, ShaderValuePtr r3)
	{
		BLT_ASSERT(r0->Type() == ValueType::Vector4f && r1->Type() == ValueType::Vector4f && r2->Type() == ValueType::Vector4f && r3->Type() == ValueType::Vector4f, "Rows must be Vector4f");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("mat4(@0,@1,@2,@3)", std::vector<ShaderValuePtr>{ std::move(r0), std::move(r1), std::move(r2), std::move(r3) }, ValueType::Matrix4f);
		return result;
	}

	ShaderFuncResultPtr ShaderFuncs::SampleTexture(ShaderValuePtr texture, ShaderValuePtr texCoords)
	{
		BLT_ASSERT(texture->Type() == ValueType::Texture2D && texCoords->Type() == ValueType::Vector2f, "Only 2D textures supported");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("texture(@0,@1)", std::vector<ShaderValuePtr>{ std::move(texture), std::move(texCoords) }, ValueType::Vector4f);
		return result;
	}

	ShaderFuncResultPtr ShaderFuncs::SampleTextureLod(ShaderValuePtr texture, ShaderValuePtr texCoords, ShaderValuePtr lod)
	{
		BLT_ASSERT(texture->Type() == ValueType::Texture2D && texCoords->Type() == ValueType::Vector2f, "Only 2D textures supported");
		BLT_ASSERT(lod->Type() == ValueType::Float, "LOD must be float");
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>("textureLod(@0,@1,@2)", std::vector<ShaderValuePtr>{ std::move(texture), std::move(texCoords), std::move(lod) }, ValueType::Vector4f);
		return result;
	}

	ShaderFuncResultPtr ShaderFuncs::Call(const FunctionScope& func, const std::vector<ShaderValuePtr>& inputs)
	{
		BLT_ASSERT(func.GetInputs().size() == inputs.size(), "Number of arguments does not match arguments of function, required inputs {0}", func.GetInputs().size());
		std::string cmd = func.GetName() + '(';
		if (inputs.size() > 0)
		{
			cmd += "@0";
			for (int i = 1; i < inputs.size(); i++)
			{
				cmd += ",@" + std::to_string(i);
			}
		}
		cmd += ')';
		ShaderFuncResultPtr result = std::make_shared<ShaderFuncResult>(cmd, inputs, func.GetReturnType().Type);
		return result;
	}

	ShaderArrayValuePtr ShaderFuncs::Index(ShaderLValuePtr arr, ShaderValuePtr index)
	{
		BLT_ASSERT(index->Type() == ValueType::Int, "Index must be an int");
		BLT_ASSERT(index->TypeDimension() == ValueTypeDim::Single, "Index cannot be an array");
		if (ValueTypeIsMatrix(arr->Type()) && arr->TypeDimension() == ValueTypeDim::Single)
		{
			ValueType resultType = ValueType::Void;
			switch (arr->Type())
			{
			case ValueType::Matrix4f:
				resultType = ValueType::Vector4f;
				break;
			case ValueType::Matrix3f:
				resultType = ValueType::Vector3f;
				break;
			case ValueType::Matrix2f:
				resultType = ValueType::Vector2f;
				break;
			default:
				break;
			}
			ShaderArrayValuePtr result = std::make_shared<ShaderArrayValue>(arr, index, resultType);
			return result;
		}
		BLT_ASSERT(arr->TypeDimension() == ValueTypeDim::Array, "Can only operate on arrays");
		ShaderArrayValuePtr result = std::make_shared<ShaderArrayValue>(arr, index);
		return result;
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