#include "Types.h"
#include "MaterialBuilder.h"
#include "Material.h"

namespace Bolt
{

	MaterialBuilder::MaterialBuilder()
		: m_Factory()
	{
	
	}

	const ShaderFactory& MaterialBuilder::Factory() const
	{
		return m_Factory;
	}

	ShaderFactory& MaterialBuilder::Factory()
	{
		return m_Factory;
	}

	std::unique_ptr<Material> MaterialBuilder::BuildMaterial() const
	{
		std::unique_ptr<Material> material = std::unique_ptr<Material>(new Material(*this, m_Factory.BuildShader()));
		return material;
	}

	std::unique_ptr<Resource> MaterialBuilder::Clone() const
	{
		BLT_ASSERT(false, "Cannot clone material builder");
		return nullptr;
	}


}