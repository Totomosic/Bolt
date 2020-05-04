#pragma once
#include "bltpch.h"
#include "ResourceFile.h"
#include "AssetHandle.h"

namespace Bolt
{

	struct BLT_API ResourcePack
	{
	private:
		std::unordered_map<std::string, ResourceFile> m_Resources;

	public:
		ResourcePack();

		const ResourceFile& GetResourceFile(const std::string& resourceName) const;
		ResourceFile& GetResourceFile(const std::string& resourceName);
		id_t GetResourceId(const std::string& resourceName) const;
		bool ResourceExists(const std::string& resourceName) const;

		friend class AssetManager;
		friend class ResourceExtractor;

	};

}