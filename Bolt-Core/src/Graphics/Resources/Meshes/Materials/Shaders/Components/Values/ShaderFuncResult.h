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
		std::string m_TemplateString;

	public:
		ShaderFuncResult(const std::string& templateString, const std::vector<ShaderValuePtr>& inputs, ValueType resultType, ValueTypeDim dim = ValueTypeDim::Single);

		virtual void Build(ShaderBuilder& builder) const override;

	};

}