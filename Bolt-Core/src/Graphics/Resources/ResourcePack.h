#pragma once
#include "bltpch.h"
#include "ResourceFile.h"

namespace Bolt
{

	struct BLT_API ResourcePack
	{
	public:
		std::unordered_map<blt::string, ResourceFile> m_Resources;

	public:
		ResourcePack();

		const ResourceFile& GetResource(const blt::string& resourceName) const;
		ResourceFile& GetResource(const blt::string& resourceName);
		id_t GetResourceId(const blt::string& resourceName) const;
		bool ResourceExists(const blt::string& resourceName) const;

		friend class ResourceManager;

	};

}