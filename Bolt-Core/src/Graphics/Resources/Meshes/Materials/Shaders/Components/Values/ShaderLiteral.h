#pragma once
#include "ShaderValue.h"

namespace Bolt
{

	class ShaderLiteral;
	using ShaderLiteralPtr = std::shared_ptr<ShaderLiteral>;

	class BLT_API ShaderLiteral : public ShaderValue
	{
	private:
		blt::string m_ValueString;

	public:
		template<typename T>
		ShaderLiteral(const T& value)
		{
			
		}

	};

}