#include "bltpch.h"

#include "AssetsLib/Image2D.h"

#include "Core/Tasks/TaskManager.h"
#include "Engine/Engine.h"
#include "AssetManager.h"
#include "Textures/Texture2D.h"
#include "Meshes/Model.h"
#include "Textures/Fonts/Font.h"
#include "Meshes/Materials/Shaders/Shader.h"
#include "Meshes/Factories/CuboidFactory.h"

namespace Bolt
{

	AssetManager& AssetManager::Get()
	{
		return Engine::Instance().CurrentContext().GetRenderContext().GetAssetManager();
	}

	AssetManager::AssetManager()
		: m_Assets(), m_Fonts(this), m_Textures(this), m_Materials(this), m_Models(this)
	{
	
	}

	const MaterialManager& AssetManager::Materials() const
	{
		return m_Materials;
	}

	const FontManager& AssetManager::Fonts() const
	{
		return m_Fonts;
	}

	const TextureManager& AssetManager::Textures() const
	{
		return m_Textures;
	}

	const BasicModels& AssetManager::Meshes() const
	{
		return m_Models;
	}

	bool AssetManager::AssetExists(const ResourceId& id) const
	{
		return m_Assets.find(id) != m_Assets.end();
	}

	ResourceId AssetManager::LoadAsset(const FilePath& assetFile)
	{
		Assets::AssetReader reader(assetFile);
		switch (reader.GetType())
		{
		case Assets::AssetType::Image2D:
			return LoadImage2D(reader);
		default:
			break;
		}
		BLT_ASSERT(false, "Invalid asset type");
		return -1;
	}

	id_t AssetManager::RegisterGetId(std::unique_ptr<Resource>&& resource)
	{
		id_t id = FindNextId();
		m_Assets[id] = std::move(resource);
		return id;
	}

	AssetHandle<Resource> AssetManager::GetAsset(const ResourceId& id)
	{
		if (AssetExists(id))
		{
			return AssetHandle<Resource>(m_Assets.at(id).get(), false);
		}
		return nullptr;
	}

	bool AssetManager::FreeAsset(const ResourceId& id)
	{
		Resource* resource = GetAsset(id).Get();
		if (resource)
		{
			m_Assets.erase(id);
			return true;
		}
		return false;
	}

	ResourceId AssetManager::FindNextId() const
	{
		ResourceId id = 0;
		while (AssetExists(id))
		{
			id++;
		}
		return id;
	}

	ResourceId AssetManager::LoadImage2D(const Assets::AssetReader& reader)
	{
		Assets::Asset<Image2D> texture = reader.ReadAsImage2D();
		ResourceId id = FindNextId();
		m_Assets[id] = std::make_unique<Texture2D>(texture.Data);
		return id;
	}

}