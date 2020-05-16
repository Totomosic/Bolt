#include "bltpch.h"
#include "TextureManager.h"
#include "AssetManager.h"

namespace Bolt
{

	TextureManager::TextureManager(AssetManager* manager)
		: m_Manager(manager), m_DefaultWhite(std::make_unique<Texture2D>(1, 1)), m_DefaultBlack(std::make_unique<Texture2D>(1, 1)), m_DefaultNormalMap(std::make_unique<Texture2D>(1, 1))
	{
		m_DefaultWhite->LoadPixels();
		m_DefaultWhite->Pixels[0] = Color::White;
		m_DefaultWhite->UpdatePixels();

		m_DefaultBlack->LoadPixels();
		m_DefaultBlack->Pixels[0] = Color::Black;
		m_DefaultBlack->UpdatePixels();

		m_DefaultNormalMap->LoadPixels();
		m_DefaultNormalMap->Pixels[0] = Color::FromFloats(0.5f, 0.5f, 1.0f);
		m_DefaultNormalMap->UpdatePixels();
	}

	AssetHandle<Texture2D> TextureManager::DefaultWhite() const
	{
		return AssetHandle<Texture2D>(m_DefaultWhite.get(), false);
	}

	AssetHandle<Texture2D> TextureManager::DefaultBlack() const
	{
		return AssetHandle<Texture2D>(m_DefaultBlack.get(), false);
	}

	AssetHandle<Texture2D> TextureManager::DefaultNormalMap() const
	{
		return AssetHandle<Texture2D>(m_DefaultNormalMap.get(), false);
	}

}