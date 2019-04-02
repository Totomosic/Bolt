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
		static std::unordered_map<ResourceID, std::unique_ptr<Resource>> s_Resources;
		static MaterialManager s_Materials;
		static FontManager s_Fonts;

	public:
		ResourceManager() = delete;

		static const MaterialManager& Materials();
		static const FontManager& Fonts();

		static ResourceFile Fetch(const Filepath& resourceFile);
		static ResourcePack FetchPack(const Filepath& resourcePack);
		static ResourceFile& LoadFile(ResourceFile& resourceFile);
		static ResourcePack& LoadPack(ResourcePack& resourcePack);

		static bool ResourceExists(const ResourceID& id);

		static id_t Register(std::unique_ptr<Resource>&& resource);
		static ResourcePtr<Resource> Get(const ResourceID& id);
		static void FreeResource(const ResourceID& id);
		static ResourcePtr<const Font> DefaultFont();

		template<typename T>
		static ResourcePtr<T> Get(const ResourceID& id)
		{
			return (ResourcePtr<T>)Get(id);
		}

		friend class Initializer;
		friend class Destructor;

	private:
		static void Initialize();
		static void Terminate();
		static id_t FindNextId();

		static ResourceType StringToType(const blt::string& str);
		static ResourceFile& LoadTexture2DFile(ResourceFile& resourceFile);
		static ResourceFile& LoadModelFile(ResourceFile& resourceFile);
		static ResourceFile& LoadShaderFile(ResourceFile& resourceFile);

	};

}