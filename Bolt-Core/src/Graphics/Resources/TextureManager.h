#pragma once
#include "Textures/Texture2D.h"
#include "ResourcePtr.h"

namespace Bolt
{

	class BLT_API TextureManager
	{
	private:
		std::unique_ptr<Texture2D> m_DefaultWhite;

	public:
		TextureManager();

		ResourcePtr<Texture2D> DefaultWhite() const;
	};

}