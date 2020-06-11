#pragma once
#include "BoltLib/IO/Filepath.h"
#include "AssetsLib/AssetReader.h"
#include "Resource.h"
#include "AssetHandle.h"
#include "MaterialManager.h"
#include "FontManager.h"
#include "TextureManager.h"
#include "BasicModels.h"

namespace Bolt
{

	using ResourceId = id_t;
	class Font;

	class BLT_API AssetManager
	{
	private:
		std::unordered_map<ResourceId, std::unique_ptr<Resource>> m_Assets;
		FontManager m_Fonts;
		TextureManager m_Textures;
		MaterialManager m_Materials;
		BasicModels m_Models;

	public:
		static AssetManager& Get();

	public:
		AssetManager();

		const MaterialManager& Materials() const;
		const FontManager& Fonts() const;
		const TextureManager& Textures() const;
		const BasicModels& Meshes() const;

		bool AssetExists(const ResourceId& id) const;
		ResourceId LoadAsset(const FilePath& assetFile);

		id_t RegisterGetId(std::unique_ptr<Resource>&& resource);
		AssetHandle<Resource> GetAsset(const ResourceId& id);
		bool FreeAsset(const ResourceId& id);

		template<typename T>
		AssetHandle<T> LoadAsset(const FilePath& assetFile)
		{
			return GetAsset<T>(LoadAsset(assetFile));
		}

		template<typename T>
		AssetHandle<T> Register(std::unique_ptr<T>&& resource)
		{
			return GetAsset<T>(RegisterGetId(std::move(resource)));
		}

		template<typename T>
		AssetHandle<T> GetAsset(const ResourceId& id)
		{
			return (AssetHandle<T>)GetAsset(id);
		}

	private:
		ResourceId FindNextId() const;

		ResourceId LoadImage2D(const Assets::AssetReader& reader);

	};

}