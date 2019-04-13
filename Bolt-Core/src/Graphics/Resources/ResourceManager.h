#pragma once
#include "Bolt-Core.h"
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
		static std::unique_ptr<ResourceManager> s_Instance;

	private:
		std::unordered_map<ResourceID, std::unique_ptr<Resource>> m_Resources;
		MaterialManager m_Materials;
		FontManager m_Fonts;

	public:
		ResourceManager();
		static ResourceManager& Get();
		static void Terminate();

	public:
		const MaterialManager& Materials();
		const FontManager& Fonts();

		ResourceFile Fetch(const Filepath& resourceFile);
		ResourcePack FetchPack(const Filepath& resourcePack);
		ResourceFile& LoadFile(ResourceFile& resourceFile);
		ResourcePack& LoadPack(ResourcePack& resourcePack);

		bool ResourceExists(const ResourceID& id);

		id_t Register(std::unique_ptr<Resource>&& resource);
		ResourcePtr<Resource> Get(const ResourceID& id);
		void FreeResource(const ResourceID& id);
		ResourcePtr<const Font> DefaultFont();

		template<typename T>
		ResourcePtr<T> Get(const ResourceID& id)
		{
			return (ResourcePtr<T>)Get(id);
		}

	private:
		id_t FindNextId();

		ResourceType StringToType(const blt::string& str);
		ResourceFile& LoadTexture2DFile(ResourceFile& resourceFile);
		ResourceFile& LoadModelFile(ResourceFile& resourceFile);
		ResourceFile& LoadShaderFile(ResourceFile& resourceFile);

	};

}