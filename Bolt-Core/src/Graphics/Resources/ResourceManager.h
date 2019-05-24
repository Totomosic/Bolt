#pragma once
#include "Resource.h"
#include "ResourceFile.h"
#include "ResourcePack.h"
#include "ResourcePtr.h"
#include "MaterialManager.h"
#include "FontManager.h"

namespace Bolt
{

	typedef id_t ResourceID;
	class Font;

	class BLT_API ResourceManager
	{
	private:
		std::unordered_map<ResourceID, std::unique_ptr<Resource>> m_Resources;
		MaterialManager m_Materials;
		FontManager m_Fonts;

	public:
		static ResourceManager& Get();

	public:
		ResourceManager();

		const MaterialManager& Materials();
		const FontManager& Fonts();

		void LoadPack(const Filepath& resourcePack, std::function<void(const ResourcePack&)> callback);

		bool ResourceExists(const ResourceID& id);

		id_t Register(std::unique_ptr<Resource>&& resource);
		ResourcePtr<Resource> GetResource(const ResourceID& id);
		void FreeResource(const ResourceID& id);
		ResourcePtr<const Font> DefaultFont();

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