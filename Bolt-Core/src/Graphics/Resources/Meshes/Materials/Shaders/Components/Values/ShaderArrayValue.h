#pragma once
#include "ShaderLValue.h"

namespace Bolt
{

	class ShaderArrayValue;
	using ShaderArrayValuePtr = std::shared_ptr<ShaderArrayValue>;

	class BLT_API ShaderArrayValue : public ShaderLValue
	{
	private:
		ShaderLValuePtr m_Array;
		ShaderValuePtr m_Index;

	public:
		ShaderArrayValue(const ShaderLValuePtr& arr, const ShaderValuePtr& index);

		void Build(ShaderBuilder& builder) const override;

	};

}