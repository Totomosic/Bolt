#pragma once
#include "ShaderLValue.h"

namespace Bolt
{

	class ShaderVariable;
	using ShaderVariablePtr = std::shared_ptr<ShaderVariable>;

	class BLT_API ShaderVariable : public ShaderLValue
	{
	protected:
		mutable blt::string m_Name;

	public:
		ShaderVariable(ValueType type, ValueTypeDim dim = ValueTypeDim::Single);

		const blt::string& GetVarName() const;
		void SetVarName(const blt::string& varName);

		virtual void Build(ShaderBuilder& builder) const override;

	};

}