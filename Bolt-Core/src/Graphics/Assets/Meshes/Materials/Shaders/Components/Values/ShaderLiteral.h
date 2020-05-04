#pragma once
#include "ShaderValue.h"

namespace Bolt
{

	class ShaderLiteral;
	using ShaderLiteralPtr = std::shared_ptr<ShaderLiteral>;

	class BLT_API ShaderLiteral : public ShaderValue
	{
	private:
		std::string m_ValueString;

	public:
		ShaderLiteral(const std::string& valueString, ValueType type, ValueTypeDim dim = ValueTypeDim::Single);

		const std::string& GetValueString() const;

		virtual void Build(ShaderBuilder& builder) const override;

	public:
		static ShaderLiteralPtr True();
		static ShaderLiteralPtr False();
		static ShaderLiteralPtr Pi();
		static ShaderLiteralPtr FromBool(bool value);
		static ShaderLiteralPtr FromInt(int value);
		static ShaderLiteralPtr FromFloat(float value);
		static ShaderLiteralPtr FromVec2(const Vector2f& value);
		static ShaderLiteralPtr FromVec3(const Vector3f& value);
		static ShaderLiteralPtr FromVec4(const Vector4f& value);

	};

}