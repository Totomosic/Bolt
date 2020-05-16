#pragma once
#include "Textures/Texture2D.h"
#include "AssetHandle.h"

namespace Bolt
{

	class AssetManager;

	class BLT_API TextureManager
	{
	private:
		AssetManager* m_Manager;

		std::unique_ptr<Texture2D> m_DefaultWhite;
		std::unique_ptr<Texture2D> m_DefaultBlack;
		std::unique_ptr<Texture2D> m_DefaultNormalMap;

	public:
		TextureManager(AssetManager* manager);

		AssetHandle<Texture2D> DefaultWhite() const;
		AssetHandle<Texture2D> DefaultBlack() const;
		AssetHandle<Texture2D> DefaultNormalMap() const;
	};

}