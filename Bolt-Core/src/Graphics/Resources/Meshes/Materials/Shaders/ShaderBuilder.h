#pragma once
#include "Components/Values/ShaderVariable.h"

namespace Bolt
{

	class BLT_API ShaderBuilder
	{
	private:
		blt::string m_Source;
		std::vector<ShaderVariablePtr> m_DeclaredVariables;

	public:
		ShaderBuilder();

		const blt::string& GetSource() const;

	};

}