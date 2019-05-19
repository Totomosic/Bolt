#include "bltpch.h"
#include "ShaderLiteral.h"
#include "../ShaderBuilder.h"

namespace Bolt
{

	ShaderLiteral::ShaderLiteral(const blt::string& valueString, ValueType type, ValueTypeDim dim) : ShaderValue(type, dim),
		m_ValueString(valueString)
	{
	
	}

	const blt::string& ShaderLiteral::GetValueString() const
	{
		return m_ValueString;
	}

	void ShaderLiteral::Build(ShaderBuilder& builder) const
	{
		builder.Write(m_ValueString);
	}

	ShaderLiteralPtr ShaderLiteral::True()
	{
		return FromBool(true);
	}

	ShaderLiteralPtr ShaderLiteral::False()
	{
		return FromBool(false);
	}

	ShaderLiteralPtr ShaderLiteral::FromBool(bool value)
	{
		return std::make_shared<ShaderLiteral>((value) ? "true" : "false", ValueType::Bool);
	}

	ShaderLiteralPtr ShaderLiteral::FromInt(int value)
	{
		return std::make_shared<ShaderLiteral>(std::to_string(value), ValueType::Int);
	}

	ShaderLiteralPtr ShaderLiteral::FromFloat(float value)
	{
		return std::make_shared<ShaderLiteral>(std::to_string(value), ValueType::Float);
	}

	ShaderLiteralPtr ShaderLiteral::FromVec2(const Vector2f& value)
	{
		return std::make_shared<ShaderLiteral>("vec4(" + std::to_string(value.x) + ", " + std::to_string(value.y) + ')', ValueType::Vector2f);
	}

	ShaderLiteralPtr ShaderLiteral::FromVec3(const Vector3f& value)
	{
		return std::make_shared<ShaderLiteral>("vec4(" + std::to_string(value.x) + ", " + std::to_string(value.y) + ", " + std::to_string(value.z) + ')', ValueType::Vector3f);
	}

	ShaderLiteralPtr ShaderLiteral::FromVec4(const Vector4f& value)
	{
		return std::make_shared<ShaderLiteral>("vec4(" + std::to_string(value.x) + ", " + std::to_string(value.y) + ", " + std::to_string(value.z) + ", " + std::to_string(value.w) + ')', ValueType::Vector4f);
	}

}