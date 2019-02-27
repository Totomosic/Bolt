#pragma once
#include "ShaderEnums.h"

namespace Bolt
{
	class BLT_API ShaderValue
	{
	private:
		OutputValueType m_Type;
		ValueType m_ValueType;
		ShaderType m_ShaderType;
		blt::string m_ValueName;

	public:
		ShaderValue();
		ShaderValue(OutputValueType type, ValueType valueType, ShaderType shader, const blt::string& valueName);

		OutputValueType OutputType() const;
		ValueType Type() const;
		ShaderType Shader() const;
		blt::string VarName() const;

		friend class ShaderProgramFactory;

	private:
		void SetShader(ShaderType shader);

	public:
		template<typename T>
		static std::shared_ptr<ShaderValue> Literal(const T& value)
		{
			BLT_ASSERT(false, "Unable to create literal for type " + typeid(value).name());
			return std::shared_ptr<ShaderValue>();
		}

		template<>
		static std::shared_ptr<ShaderValue> Literal(const int& value)
		{
			return std::make_shared<ShaderValue>(OutputValueType::Constant, ValueType::Int, ShaderType::Ignore, std::to_string(value));
		}

		template<>
		static std::shared_ptr<ShaderValue> Literal(const float& value)
		{
			return std::make_shared<ShaderValue>(OutputValueType::Constant, ValueType::Float, ShaderType::Ignore, std::to_string(value));
		}

		template<>
		static std::shared_ptr<ShaderValue> Literal(const Vector2f& value)
		{
			return std::make_shared<ShaderValue>(OutputValueType::Constant, ValueType::Vector2f, ShaderType::Ignore, "vec2(" + std::to_string(value.x) + ',' + std::to_string(value.y) + ')');
		}

		template<>
		static std::shared_ptr<ShaderValue> Literal(const Vector2i& value)
		{
			return std::make_shared<ShaderValue>(OutputValueType::Constant, ValueType::Vector2i, ShaderType::Ignore, "vec2(" + std::to_string(value.x) + ',' + std::to_string(value.y) + ')');
		}

		template<>
		static std::shared_ptr<ShaderValue> Literal(const Vector3f& value)
		{
			return std::make_shared<ShaderValue>(OutputValueType::Constant, ValueType::Vector3f, ShaderType::Ignore, "vec3(" + std::to_string(value.x) + ',' + std::to_string(value.y) + ',' + std::to_string(value.z) + ')');
		}

		template<>
		static std::shared_ptr<ShaderValue> Literal(const Vector3i& value)
		{
			return std::make_shared<ShaderValue>(OutputValueType::Constant, ValueType::Vector3i, ShaderType::Ignore, "vec3(" + std::to_string(value.x) + ',' + std::to_string(value.y) + ',' + std::to_string(value.z) + ')');
		}

		template<>
		static std::shared_ptr<ShaderValue> Literal(const Vector4f& value)
		{
			return std::make_shared<ShaderValue>(OutputValueType::Constant, ValueType::Vector4f, ShaderType::Ignore, "vec4(" + std::to_string(value.x) + ',' + std::to_string(value.y) + ',' + std::to_string(value.z) + ',' + std::to_string(value.w) + ')');
		}

		template<>
		static std::shared_ptr<ShaderValue> Literal(const Vector4i& value)
		{
			return std::make_shared<ShaderValue>(OutputValueType::Constant, ValueType::Vector4i, ShaderType::Ignore, "vec4(" + std::to_string(value.x) + ',' + std::to_string(value.y) + ',' + std::to_string(value.z) + ',' + std::to_string(value.w) + ')');
		}

	};

	using ShaderValuePtr = std::shared_ptr<ShaderValue>;

}