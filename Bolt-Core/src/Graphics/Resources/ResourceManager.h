#pragma once
#include "Bolt-Core.h"
#include "Resource.h"
#include "ResourceFile.h"
#include "ResourcePack.h"
#include "ResourcePtr.h"

namespace Bolt
{

	typedef id_t ResourceID;

	class BLT_API ResourceManager
	{
	private:
		static std::unordered_map<ResourceID, std::unique_ptr<Resource>> s_Resources;

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

		template<typename T>
		static ResourcePtr<T> Get(const ResourceID& id)
		{
			return (ResourcePtr<T>)Get(id);
		}

		template<typename T>
		static id_t Register(std::unique_ptr<T>&& resource)
		{
			return Register(std::unique_ptr<Resource>(resource.release()));
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