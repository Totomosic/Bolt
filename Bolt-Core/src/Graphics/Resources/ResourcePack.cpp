#include "Types.h"
#include "ResourcePack.h"

namespace Bolt
{

	ResourcePack::ResourcePack()
		: m_Resources()
	{

	}

	const ResourceFile& ResourcePack::GetResource(const blt::string& resourceName) const
	{
		return m_Resources.at(resourceName);
	}

	ResourceFile& ResourcePack::GetResource(const blt::string& resourceName)
	{
		return m_Resources.at(resourceName);
	}

	id_t ResourcePack::GetResourceId(const blt::string& resourceName) const
	{
		return GetResource(resourceName).Id;
	}

	bool ResourcePack::ResourceExists(const blt::string& resourceName) const
	{
		return m_Resources.find(resourceName) != m_Resources.end();
	}

}