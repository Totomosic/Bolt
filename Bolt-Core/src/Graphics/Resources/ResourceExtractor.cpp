#include "bltpch.h"
#include "ResourceExtractor.h"

namespace Bolt
{

	ResourceExtractor::ResourceExtractor(const ResourcePack& resources)
		: m_Resources(resources)
	{
	
	}

	const ResourcePack& ResourceExtractor::GetPack() const
	{
		return m_Resources;
	}

	id_t ResourceExtractor::GetResourceId(const std::string& name) const
	{
		return m_Resources.GetResourceId(name);
	}

	std::vector<ResourceExtractor::ResourceInfo> ResourceExtractor::GetResources() const
	{
		std::vector<ResourceInfo> result;
		for (const auto& pair : m_Resources.m_Resources)
		{
			ResourceInfo info;
			info.Name = pair.first;
			info.Id = pair.second.Id;
			info.Type = pair.second.Type;
			result.push_back(info);
		}
		return result;
	}

}