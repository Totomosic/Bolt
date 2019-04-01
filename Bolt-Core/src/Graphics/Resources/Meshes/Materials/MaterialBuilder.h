#pragma once
#include "Modules/__Modules__.h"
#include "Material.h"

namespace Bolt
{

	class BLT_API MaterialBuilder : public Resource
	{
	private:
		ShaderFactory m_Factory;

	public:
		MaterialBuilder();

		const ShaderFactory& Factory() const;
		ShaderFactory& Factory();

		std::unique_ptr<Material> BuildMaterial() const;

		std::unique_ptr<Resource> Clone() const override;

	};

}