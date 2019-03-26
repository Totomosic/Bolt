#pragma once
#include "ShaderValue.h"

namespace Bolt
{

	class ShaderVariable;
	using ShaderVariablePtr = std::shared_ptr<ShaderVariable>;

	class BLT_API ShaderVariable : public ShaderValue
	{
	protected:
		mutable blt::string m_Name;
		ShaderValuePtr m_Value;
		mutable ShaderType m_ShaderType;

	public:
		ShaderVariable(ValueType type);
		ShaderVariable(ShaderValuePtr value);

		ShaderType GetShaderType() const;
		const blt::string& GetVarName() const;

		virtual void Build(ShaderBuilder& builder) const override;

		friend class ShaderBuilder;

	public:
		static ShaderVariablePtr Create(ShaderValuePtr value);

	};

}