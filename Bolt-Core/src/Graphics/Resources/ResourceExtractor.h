#pragma once
#include "ResourceManager.h"

namespace Bolt
{

	class BLT_API ResourceExtractor
	{
	public:
		struct BLT_API ResourceInfo
		{
		public:
			std::string Name;
			id_t Id;
			ResourceType Type;
		};

	private:
		const ResourcePack& m_Resources;

	public:
		// Does not own the resources pack
		ResourceExtractor(const ResourcePack& resources);
		
		const ResourcePack& GetPack() const;		
		id_t GetResourceId(const std::string& name) const;
		std::vector<ResourceInfo> GetResources() const;

		template<typename T>
		ResourcePtr<T> GetResourcePtr(const std::string& name) const
		{
			return ResourceManager::Get().GetResource<T>(m_Resources.GetResourceId(name));
		}
	
	};

}