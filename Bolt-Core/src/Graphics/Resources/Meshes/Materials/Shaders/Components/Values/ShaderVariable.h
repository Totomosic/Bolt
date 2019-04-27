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
		mutable ShaderType m_ShaderType;

	public:
		ShaderVariable(ValueType type);

		ShaderType GetShaderType() const;
		const blt::string& GetVarName() const;

		virtual void Build(ShaderBuilder& builder) const override;

		friend class ShaderDefineOp;
		friend class ShaderDeclareOp;
		friend class VertexShader;
		friend class DeclarePassInOp;
		friend class DeclarePassOutOp;

	};

}