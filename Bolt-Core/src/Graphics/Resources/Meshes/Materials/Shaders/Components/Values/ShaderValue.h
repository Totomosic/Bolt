#pragma once
#include "ShaderEnums.h"

namespace Bolt
{

	class ShaderValue;
	using ShaderValuePtr = std::shared_ptr<ShaderValue>;
	class ShaderBuilder;

	class BLT_API ShaderValue
	{
	protected:
		ValueType m_ValueType;

	public:
		ShaderValue();
		ShaderValue(ValueType type);

		ValueType Type() const;

		virtual void Build(ShaderBuilder& builder) const = 0;

		friend class ShaderProgramFactory;

	};

}