#pragma once
#include "Shaders/ShaderFactory.h"
#include "Graphics/Resources/Resource.h"

namespace Bolt
{

	class Material;

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