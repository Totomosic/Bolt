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

		template<typename MaterialT>
		std::unique_ptr<MaterialT> BuildMaterial() const
		{
			std::unique_ptr<MaterialT> material = std::unique_ptr<MaterialT>(new MaterialT(m_Factory.BuildShader(), IsTransparent()));
			return material;
		}

	};

}