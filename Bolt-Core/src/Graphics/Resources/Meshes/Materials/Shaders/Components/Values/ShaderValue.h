#pragma once
#include "ShaderEnums.h"

namespace Bolt
{

	class ShaderValue;
	using ShaderValuePtr = std::shared_ptr<ShaderValue>;
	using ShaderValueNameFunc = std::function<blt::string(const ShaderValue*)>;

	class BLT_API ShaderValue
	{
	protected:
		ValueType m_ValueType;

	public:
		ShaderValue();
		ShaderValue(ValueType type);

		ValueType Type() const;

		virtual blt::string GetDeclString(const ShaderValueNameFunc& getValueNameFunc) const;

		friend class ShaderProgramFactory;

	};

}