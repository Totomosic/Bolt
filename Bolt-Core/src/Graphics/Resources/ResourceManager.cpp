#include "Types.h"
#include "ResourceManager.h"
#include "Textures\Texture2D.h"
#include "Meshes\Model.h"
#include "Textures\Fonts\Font.h"

namespace Bolt
{

	std::unordered_map<ResourceID, std::unique_ptr<Resource>> ResourceManager::s_Resources = std::unordered_map<ResourceID, std::unique_ptr<Resource>>();
	Font* ResourceManager::s_DefaultFont = nullptr;

	ResourceFile ResourceManager::Fetch(const Filepath& resourceFile)
	{
		XMLfile file = Filesystem::OpenXML(resourceFile);
		ResourceFile result;
		XMLnode root = file.LoadXML();
		result.Name = root.Attributes.at("name");
		result.Type = StringToType(root.Name);
		result.Attributes = root;
		return result;
	}

	ResourcePack ResourceManager::FetchPack(const Filepath& resourcePack)
	{
		XMLfile file = Filesystem::OpenXML(resourcePack);
		XMLnode root = file.LoadXML();
		ResourcePack result;
		for (XMLnode& resource : root.Children)
		{
			ResourceFile resFile;
			resFile.Name = resource.Attributes.at("name");
			resFile.Type = StringToType(resource.Name);
			resFile.Attributes = resource;
			result.Resources[resFile.Name] = std::move(resFile);
		}
		return result;
	}

	ResourceFile& ResourceManager::LoadFile(ResourceFile& resourceFile)
	{
		switch (resourceFile.Type)
		{
		case ResourceType::Texture2D:
			return LoadTexture2DFile(resourceFile);
		case ResourceType::Model:
			return LoadModelFile(resourceFile);
		}
		BLT_ASSERT(false, "Unable to load Resource File " + resourceFile.Name);
		return resourceFile;
	}

	ResourcePack& ResourceManager::LoadPack(ResourcePack& resourcePack)
	{
		for (auto& pair : resourcePack.Resources)
		{
			pair.second = LoadFile(pair.second);
		}
		return resourcePack;
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
		if (resource == s_DefaultFont)
		{
			s_DefaultFont = nullptr;
		}
		s_Resources.erase(id);
	}

	ResourcePtr<Font> ResourceManager::DefaultFont()
	{
		return ResourcePtr<Font>(s_DefaultFont, false);
	}

	void ResourceManager::SetDefaultFont(id_t font)
	{
		s_DefaultFont = Get<Font>(font).Get();
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
		if (str == "TEXTURE2D")
		{
			return ResourceType::Texture2D;
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

	ResourceFile& ResourceManager::LoadTexture2DFile(ResourceFile& resourceFile)
	{
		int width = std::stoi(resourceFile.Attributes.GetChild("width").Data.c_str());
		int height = std::stoi(resourceFile.Attributes.GetChild("height").Data.c_str());
		const char* data = resourceFile.Attributes.GetChild("data").Data.data();
		const blt::string& magString = resourceFile.Attributes.GetChild("options").Attributes.at("magnification");
		const blt::string& minString = resourceFile.Attributes.GetChild("options").Attributes.at("minification");
		const blt::string& mipmapString = resourceFile.Attributes.GetChild("options").Attributes.at("mipmaps");
		Image image;
		image.Width = width;
		image.Height = height;
		image.Components = 4;
		image.Pixels = (byte*)data;
		TextureCreateOptions options;
		options.Magnification = (magString == "nearest") ? MagFilter::Nearest : MagFilter::Linear;
		options.Minification = (minString == "nearest") ? MinFilter::Nearest : MinFilter::Linear;
		options.MipmapMode = (mipmapString == "false") ? Mipmaps::Disabled : Mipmaps::Enabled;
		std::unique_ptr<Texture2D> texture = std::make_unique<Texture2D>(image, options);
		image.Pixels = nullptr;
		id_t id = Register(std::move(texture));
		resourceFile.Id = id;
		return resourceFile;
	}

	ResourceFile& ResourceManager::LoadModelFile(ResourceFile& resourceFile)
	{
		int vertexDimension = std::stoi(resourceFile.Attributes.GetChild("vertices").Attributes.at("dim").c_str());
		std::vector<blt::string> verticesS = resourceFile.Attributes.GetChild("vertices").Data.split(' ');
		std::vector<blt::string> normalsS = resourceFile.Attributes.GetChild("normals").Data.split(' ');
		std::vector<blt::string> texcoordsS = resourceFile.Attributes.GetChild("texcoords").Data.split(' ');
		blt::string colors = resourceFile.Attributes.GetChild("colors").Data;
		std::vector<blt::string> indicesS = resourceFile.Attributes.GetChild("indices").Data.split(' ');
		std::vector<float> vertices;
		std::vector<float> normals;
		std::vector<float> texcoords;
		std::vector<uint> indices;
		vertices.resize(verticesS.size());
		normals.resize(normalsS.size());
		texcoords.resize(texcoordsS.size());
		indices.resize(indicesS.size());
		std::transform(verticesS.begin(), verticesS.end(), vertices.begin(), [](const blt::string& str) {
			return std::stof(str.c_str());
		});
		std::transform(normalsS.begin(), normalsS.end(), normals.begin(), [](const blt::string& str) {
			return std::stof(str.c_str());
		});
		std::transform(texcoordsS.begin(), texcoordsS.end(), texcoords.begin(), [](const blt::string& str) {
			return std::stof(str.c_str());
		});
		std::transform(indicesS.begin(), indicesS.end(), indices.begin(), [](const blt::string& str) {
			return (uint)std::stoi(str.c_str());
		});

		ModelData data;
		data.Indices.AddIndexBuffer(std::make_unique<IndexBuffer>(indices.data(), indices.size()));
		data.Vertices = std::make_unique<VertexArray>();
		BufferLayout layout = BufferLayout::Default();
		VertexBuffer& buffer = data.Vertices->CreateVertexBuffer(vertices.size() / vertexDimension * layout.Size(), layout);
		
		{
			VertexIterator it = data.Vertices->Begin();
			for (int vertex = 0; vertex < vertices.size() / vertexDimension; vertex++)
			{
				int vIndex = vertex * vertexDimension;
				int nIndex = vertex * 3;
				int tIndex = vertex * 2;
				int cIndex = vertex * 4;

				it[0] = Vector3f(vertices[vIndex + 0], vertices[vIndex + 1], vertices[vIndex + 2]);
				it[1] = Vector3f(normals[nIndex + 0], normals[nIndex + 1], normals[nIndex + 2]);
				it[2] = Vector2f(texcoords[tIndex + 0], texcoords[tIndex + 1]);
				it[3] = Vector4<byte>(colors[cIndex + 0], colors[cIndex + 1], colors[cIndex + 2], colors[cIndex + 2]);
				it++;
			}
		}

		std::unique_ptr<Model> model = std::make_unique<Model>(std::move(data));
		id_t id = Register(std::move(model));
		resourceFile.Id = id;

		return resourceFile;
	}

}