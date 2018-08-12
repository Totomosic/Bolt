#pragma once
#include "Bolt-Core.h"
#include "Resource.h"

namespace Bolt
{

	typedef blt::string ResourceID;

	class BLT_API ResourceManager
	{
	private:
		static std::unordered_map<ResourceID, std::unique_ptr<Resource>> s_Resources;

	public:
		ResourceManager() = delete;

		static bool ResourceExists(const ResourceID& id);

		static Resource* Register(const ResourceID& id, std::unique_ptr<Resource>&& resource);
		static Resource* Get(const ResourceID& id);
		static void FreeResource(const ResourceID& id);

		template<typename T>
		static T* Get(const ResourceID& id)
		{
			return (T*)Get(id);
		}

		template<typename T>
		static T* Register(const ResourceID& id, std::unique_ptr<T>&& resource)
		{
			return (T*)Register(id, std::unique_ptr<Resource>(resource.release()));
		}

		friend class Engine;

	private:
		static void Terminate();

	};

}