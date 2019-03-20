#pragma once
#include "ShaderVariable.h"

namespace Bolt
{

	class ShaderAttribute;
	using ShaderAttributePtr = std::shared_ptr<ShaderAttribute>;

	class BLT_API ShaderAttribute : public ShaderVariable
	{
	public:
		static constexpr int VERTEX_POSITION = 0;
		static constexpr int FRAG_COLOR = 1;

	private:
		int m_Attribute;

	public:
		// Value can be nullptr for attributes that can be read
		ShaderAttribute(int attribute, ShaderValuePtr value);

		virtual void Build(ShaderBuilder& builder) const override;

		blt::string GetName() const;

	};

}