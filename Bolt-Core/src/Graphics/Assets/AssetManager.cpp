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
		File f = Filesystem::Open(assetFile, OpenMode::Read);
		size_t size = f.GetSize();
		char* buffer = new char[size];
		f.Read(buffer, size);
		Assets::AssetHeader header = Assets::AssetHeader::Deserialize((const void*)buffer);
		switch (header.Type)
		{
		case Assets::AssetType::Texture2D:
			return LoadTexture2DAsset((const void*)buffer, size);
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

	ResourceId AssetManager::LoadTexture2DAsset(const void* data, size_t length)
	{
		Assets::Asset<Image2D> texture = Assets::Texture2DEngine::ReadBoltFormat(data, length);
		ResourceId id = FindNextId();
		m_Assets[id] = std::make_unique<Texture2D>(texture.Data);
		return id;
	}

}