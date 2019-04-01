#pragma once
#include "Meshes/Materials/__Materials__.h"

namespace Bolt
{

	class BLT_API MaterialManager
	{
	private:
		MaterialBuilder m_DefaultBuilder;
		
	public:
		MaterialManager();

		std::unique_ptr<Material> Default(const Color& baseColor = Color::White) const;

	private:
		void CreateDefaultMaterial(MaterialBuilder& builder) const;

	};

}