#include "bltpch.h"
#include "TextureManager.h"
#include "ResourceManager.h"

namespace Bolt
{

	TextureManager::TextureManager()
		: m_DefaultWhite(nullptr)
	{
		m_DefaultWhite = std::make_unique<Texture2D>(1, 1);
		m_DefaultWhite->LoadPixels();
		m_DefaultWhite->Pixels[0] = Color::White;
		m_DefaultWhite->UpdatePixels();
	}

	ResourcePtr<Texture2D> TextureManager::DefaultWhite() const
	{
		return ResourcePtr<Texture2D>(m_DefaultWhite.get(), false);
	}

}