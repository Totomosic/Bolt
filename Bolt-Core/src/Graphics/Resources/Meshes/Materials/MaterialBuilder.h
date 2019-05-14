#pragma once
#include "Shaders/ShaderFactory.h"

namespace Bolt
{

	class Material;

	class BLT_API MaterialBuilder
	{
	private:
		ShaderFactory m_Factory;
		bool m_IsTransparent;

	public:
		MaterialBuilder(bool isTransparent = false);

		bool IsTransparent() const;
		const ShaderFactory& Factory() const;
		ShaderFactory& Factory();
		void SetIsTransparent(bool isTransparent);

		std::unique_ptr<Material> BuildMaterial() const;

	};

}