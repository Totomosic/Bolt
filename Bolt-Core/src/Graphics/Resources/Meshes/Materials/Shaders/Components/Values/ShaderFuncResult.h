#pragma once
#include "ShaderValue.h"

namespace Bolt
{

	class ShaderFuncResult;
	using ShaderFuncResultPtr = std::shared_ptr<ShaderFuncResult>;

	class BLT_API ShaderFuncResult : public ShaderValue
	{
	public:
		static constexpr char INPUT_TEMPLATE = '@';

	private:
		std::vector<ShaderValuePtr> m_Inputs;
		blt::string m_TemplateString;

	public:
		ShaderFuncResult(const blt::string& templateString, const std::vector<ShaderValuePtr>& inputs, ValueType resultType);

		virtual void Build(ShaderBuilder& builder) const override;

	};

}