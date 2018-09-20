#include "ResourceManager.h"
#include "Textures\Texture2D.h"

namespace Bolt
{

	std::unordered_map<ResourceID, std::unique_ptr<Resource>> ResourceManager::s_Resources = std::unordered_map<ResourceID, std::unique_ptr<Resource>>();

	ResourceFile ResourceManager::Fetch(const Filepath& resourceFile)
	{
		File f = Filesystem::Open(resourceFile, OpenMode::Read);
		blt::string data = f.ReadText();
		std::vector<blt::string> parts = data.split(',');
		ResourceFile resource;
		for (auto& part : parts)
		{
			std::vector<blt::string> pair = part.split('=');
			BLT_ASSERT(pair.size() == 2, "ResourceFile syntax error");
			blt::string& key = pair[0];
			blt::string& value = pair[1];
			resource.Attributes[key] = value;
			if (key == "res")
			{
				resource.Type = StringToType(value);
			}
			if (key == "name")
			{
				resource.Name = value;
			}
		}
		return std::move(resource);
	}

	id_t ResourceManager::LoadFile(const ResourceFile& resourceFile)
	{
		switch (resourceFile.Type)
		{
		case ResourceType::Image:
			return LoadImageFile(resourceFile);
		}
		BLT_ASSERT(false, "Unable to load Resource File");
	}

	bool ResourceManager::ResourceExists(const ResourceID& id)
	{
		return s_Resources.find(id) != s_Resources.end();
	}

	id_t ResourceManager::Register(std::unique_ptr<Resource>&& resource)
	{
		id_t id = FindNextId();
		s_Resources[id] = std::move(resource);
		return id;
	}

	ResourcePtr<Resource> ResourceManager::Get(const ResourceID& id)
	{
		if (ResourceExists(id))
		{
			return ResourcePtr<Resource>(s_Resources.at(id).get(), false);
		}
		return nullptr;
	}

	void ResourceManager::FreeResource(const ResourceID& id)
	{
		Resource* resource = Get(id).Get();
		s_Resources.erase(id);
	}

	void ResourceManager::Terminate()
	{
		for (auto& pair : s_Resources)
		{
			pair.second.reset();
		}
	}

	id_t ResourceManager::FindNextId()
	{
		id_t id = 0;
		while (ResourceExists(id))
		{
			id++;
		}
		return id;
	}

	ResourceType ResourceManager::StringToType(const blt::string& str)
	{
		if (str == "IMAGE")
		{
			return ResourceType::Image;
		}
		else if (str == "MODEL")
		{
			return ResourceType::Model;
		}
		else if (str == "DATA")
		{
			return ResourceType::Data;
		}
		return ResourceType::Unknown;
	}

	id_t ResourceManager::LoadImageFile(const ResourceFile& resourceFile)
	{
		int width = std::stoi(resourceFile.Attributes.at("w").c_str());
		int height = std::stoi(resourceFile.Attributes.at("h").c_str());
		const char* data = resourceFile.Attributes.at("data").data() + 1;
		Image image;
		image.Width = width;
		image.Height = height;
		image.Components = 4;
		image.Pixels = (byte*)data;
		std::unique_ptr<Texture2D> texture = std::make_unique<Texture2D>(image);
		image.Pixels = nullptr;
		id_t id = Register(std::move(texture));
		resourceFile.Id = id;
		return id;
	}

}