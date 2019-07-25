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
		ValueTypeDim m_Dimension;

	public:
		ShaderValue();
		ShaderValue(ValueType type, ValueTypeDim dim);

		ValueType Type() const;
		ValueTypeDim TypeDimension() const;

		virtual void Build(ShaderBuilder& builder) const = 0;

		friend class ShaderProgramFactory;

	};

}