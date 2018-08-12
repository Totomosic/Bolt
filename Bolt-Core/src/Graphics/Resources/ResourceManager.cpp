#include "ResourceManager.h"

namespace Bolt
{

	std::unordered_map<ResourceID, std::unique_ptr<Resource>> ResourceManager::s_Resources = std::unordered_map<ResourceID, std::unique_ptr<Resource>>();

	bool ResourceManager::ResourceExists(const ResourceID& id)
	{
		return s_Resources.find(id) != s_Resources.end();
	}

	Resource* ResourceManager::Register(const ResourceID& id, std::unique_ptr<Resource>&& resource)
	{
		if (ResourceExists(id))
		{
			FreeResource(id);
		}
		s_Resources[id] = std::move(resource);
		return s_Resources[id].get();
	}

	Resource* ResourceManager::Get(const ResourceID& id)
	{
		if (ResourceExists(id))
		{
			return s_Resources.at(id).get();
		}
		return nullptr;
	}

	void ResourceManager::FreeResource(const ResourceID& id)
	{
		Resource* resource = Get(id);
		s_Resources.erase(id);
	}

	void ResourceManager::Terminate()
	{
		for (auto& pair : s_Resources)
		{
			pair.second.reset();
		}
	}

}