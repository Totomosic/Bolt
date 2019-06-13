#pragma once
#include "Resource.h"
#include "ResourceFile.h"
#include "ResourcePack.h"
#include "ResourcePtr.h"
#include "MaterialManager.h"
#include "FontManager.h"
#include "TextureManager.h"

namespace Bolt
{

	typedef id_t ResourceID;
	class Font;

	class BLT_API ResourceManager
	{
	private:
		std::unordered_map<ResourceID, std::unique_ptr<Resource>> m_Resources;
		FontManager m_Fonts;
		TextureManager m_Textures;
		MaterialManager m_Materials;

	public:
		static ResourceManager& Get();

	public:
		ResourceManager();

		const MaterialManager& Materials() const;
		const FontManager& Fonts() const;
		const TextureManager& Textures() const;

		void LoadPack(const Filepath& resourcePack, std::function<void(const ResourcePack&)> callback);

		bool ResourceExists(const ResourceID& id);

		id_t RegisterGetId(std::unique_ptr<Resource>&& resource);
		ResourcePtr<Resource> GetResource(const ResourceID& id);
		void FreeResource(const ResourceID& id);
		ResourcePtr<Font> DefaultFont();

		template<typename T>
		ResourcePtr<T> Register(std::unique_ptr<T>&& resource)
		{
			return GetResource<T>(RegisterGetId(std::move(resource)));
		}

		template<typename T>
		ResourcePtr<T> GetResource(const ResourceID& id)
		{
			return (ResourcePtr<T>)GetResource(id);
		}

	private:
		id_t FindNextId();
		ResourceType StringToType(const blt::string& str);

		void LoadFile(ResourceFile& resourceFile);
		void LoadTexture2DFile(ResourceFile& resourceFile);
		void LoadModelFile(ResourceFile& resourceFile);

	};

}