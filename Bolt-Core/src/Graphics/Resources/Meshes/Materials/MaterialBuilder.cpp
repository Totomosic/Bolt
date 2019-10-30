#include "bltpch.h"
#include "MaterialBuilder.h"
#include "Material.h"

namespace Bolt
{

	MaterialBuilder::MaterialBuilder(bool isTransparent)
		: m_Factory(), m_IsTransparent(isTransparent)
	{
	
	}

	bool MaterialBuilder::IsTransparent() const
	{
		return m_IsTransparent;
	}

	const ShaderFactory& MaterialBuilder::Factory() const
	{
		return m_Factory;
	}

	ShaderFactory& MaterialBuilder::Factory()
	{
		return m_Factory;
	}

	void MaterialBuilder::SetIsTransparent(bool isTransparent)
	{
		m_IsTransparent = isTransparent;
	}

}