#pragma once
#include "ShaderLValue.h"

namespace Bolt
{

	class ShaderVariable;
	using ShaderVariablePtr = std::shared_ptr<ShaderVariable>;

	class BLT_API ShaderVariable : public ShaderLValue
	{
	protected:
		mutable std::string m_Name;

	public:
		ShaderVariable(ValueType type, ValueTypeDim dim = ValueTypeDim::Single);

		const std::string& GetVarName() const;
		void SetVarName(const std::string& varName);

		virtual void Build(ShaderBuilder& builder) const override;

	};

	class ShaderPassVariable;
	using ShaderPassVariablePtr = std::shared_ptr<ShaderPassVariable>;

	class BLT_API ShaderPassVariable : public ShaderVariable
	{
	private:
		PassType m_PassType;

	public:
		ShaderPassVariable(ValueType type, ValueTypeDim dim = ValueTypeDim::Single, PassType passType = PassType::None);

		PassType GetPassType() const;
	};

}