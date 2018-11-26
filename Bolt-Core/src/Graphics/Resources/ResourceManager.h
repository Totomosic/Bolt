#pragma once
#include "Bolt-Core.h"
#include "Resource.h"
#include "ResourceFile.h"
#include "ResourcePack.h"
#include "ResourcePtr.h"

namespace Bolt
{

	typedef id_t ResourceID;
	class Font;

	class BLT_API ResourceManager
	{
	private:
		static std::unordered_map<ResourceID, std::unique_ptr<Resource>> s_Resources;
		static Font* s_DefaultFont;

	public:
		ResourceManager() = delete;

		static ResourceFile Fetch(const Filepath& resourceFile);
		static ResourcePack FetchPack(const Filepath& resourcePack);
		static ResourceFile& LoadFile(ResourceFile& resourceFile);
		static ResourcePack& LoadPack(ResourcePack& resourcePack);

		static bool ResourceExists(const ResourceID& id);

		static id_t Register(std::unique_ptr<Resource>&& resource);
		static ResourcePtr<Resource> Get(const ResourceID& id);
		static void FreeResource(const ResourceID& id);
		static ResourcePtr<Font> DefaultFont();
		static void SetDefaultFont(id_t font);

		template<typename T>
		static ResourcePtr<T> Get(const ResourceID& id)
		{
			return (ResourcePtr<T>)Get(id);
		}

		static id_t Register(std::unique_ptr<Font>&& resource)
		{
			if (s_DefaultFont == nullptr)
			{
				s_DefaultFont = resource.get();
			}
			return Register((std::unique_ptr<Resource>&&)std::move(resource));
		}

		friend class Engine;

	private:
		static void Terminate();
		static id_t FindNextId();

		static ResourceType StringToType(const blt::string& str);
		static ResourceFile& LoadTexture2DFile(ResourceFile& resourceFile);
		static ResourceFile& LoadModelFile(ResourceFile& resourceFile);

	};

}