#pragma once
#include "ShaderAttribute.h"

namespace Bolt
{

	class FragColorAttribute;
	using FragColorAttributePtr = std::shared_ptr<FragColorAttribute>;

	class BLT_API FragColorAttribute : public ShaderAttribute
	{
	public:
		FragColorAttribute(ShaderValuePtr value);

		virtual void Build(ShaderBuilder& builder) const override;

	};

}