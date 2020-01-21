#include "bltpch.h"
#include "ResourcePack.h"

namespace Bolt
{

	ResourcePack::ResourcePack()
		: m_Resources()
	{

	}

	const ResourceFile& ResourcePack::GetResourceFile(const std::string& resourceName) const
	{
		BLT_ASSERT(m_Resources.find(resourceName) != m_Resources.end(), "No resource exists named " + resourceName);
		return m_Resources.at(resourceName);
	}

	ResourceFile& ResourcePack::GetResourceFile(const std::string& resourceName)
	{
		BLT_ASSERT(m_Resources.find(resourceName) != m_Resources.end(), "No resource exists named " + resourceName);
		return m_Resources.at(resourceName);
	}

	id_t ResourcePack::GetResourceId(const std::string& resourceName) const
	{
		return GetResourceFile(resourceName).Id;
	}

	bool ResourcePack::ResourceExists(const std::string& resourceName) const
	{
		return m_Resources.find(resourceName) != m_Resources.end();
	}

}