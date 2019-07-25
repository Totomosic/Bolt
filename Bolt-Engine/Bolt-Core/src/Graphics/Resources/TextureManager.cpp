#include "bltpch.h"
#include "TextureManager.h"
#include "ResourceManager.h"

namespace Bolt
{

	TextureManager::TextureManager(ResourceManager* manager)
		: m_Manager(manager), m_DefaultWhite(std::make_unique<Texture2D>(1, 1)), m_DefaultBlack(std::make_unique<Texture2D>(1, 1))
	{
		m_DefaultWhite->LoadPixels();
		m_DefaultWhite->Pixels[0] = Color::White;
		m_DefaultWhite->UpdatePixels();

		m_DefaultBlack->LoadPixels();
		m_DefaultBlack->Pixels[0] = Color::Black;
		m_DefaultBlack->UpdatePixels();
	}

	ResourcePtr<Texture2D> TextureManager::DefaultWhite() const
	{
		return ResourcePtr<Texture2D>(m_DefaultWhite.get(), false);
	}

	ResourcePtr<Texture2D> TextureManager::DefaultBlack() const
	{
		return ResourcePtr<Texture2D>(m_DefaultBlack.get(), false);
	}

}