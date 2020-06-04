#pragma once
#include "Resource.h"
#include "ResourceFile.h"
#include "ResourcePack.h"
#include "AssetHandle.h"
#include "MaterialManager.h"
#include "FontManager.h"
#include "TextureManager.h"
#include "BasicModels.h"

namespace Bolt
{

	typedef id_t ResourceID;
	class Font;

	class BLT_API AssetManager
	{
	private:
		std::unordered_map<ResourceID, std::unique_ptr<Resource>> m_Resources;
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

		void LoadPack(const FilePath& resourcePack, std::function<void(const ResourcePack&)> callback);

		bool ResourceExists(const ResourceID& id);

		id_t RegisterGetId(std::unique_ptr<Resource>&& resource);
		AssetHandle<Resource> GetResource(const ResourceID& id);
		void FreeResource(const ResourceID& id);

		template<typename T>
		AssetHandle<T> Register(std::unique_ptr<T>&& resource)
		{
			return GetResource<T>(RegisterGetId(std::move(resource)));
		}

		template<typename T>
		AssetHandle<T> GetResource(const ResourceID& id)
		{
			return (AssetHandle<T>)GetResource(id);
		}

	private:
		id_t FindNextId();
		ResourceType StringToType(const std::string& str);

		void LoadFile(ResourceFile& resourceFile);
		void LoadTexture2DFile(ResourceFile& resourceFile);
		void LoadModelFile(ResourceFile& resourceFile);

	};

}