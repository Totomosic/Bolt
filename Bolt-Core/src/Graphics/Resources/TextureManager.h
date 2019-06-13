#pragma once
#include "Textures/Texture2D.h"
#include "ResourcePtr.h"

namespace Bolt
{

	class BLT_API TextureManager
	{
	private:
		std::unique_ptr<Texture2D> m_DefaultWhite;
		std::unique_ptr<Texture2D> m_DefaultBlack;

	public:
		TextureManager();

		ResourcePtr<Texture2D> DefaultWhite() const;
		ResourcePtr<Texture2D> DefaultBlack() const;
	};

}