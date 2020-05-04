#include "bltpch.h"

#include "Core/Tasks/TaskManager.h"
#include "Engine/Engine.h"
#include "AssetManager.h"
#include "Textures/Texture2D.h"
#include "Meshes/Model.h"
#include "Textures/Fonts/Font.h"
#include "Meshes/Materials/Shaders/Shader.h"

#include "Meshes/Factories/CuboidFactory.h"

namespace Bolt
{

	AssetManager& AssetManager::Get()
	{
		return Engine::Instance().CurrentContext().GetRenderContext().GetAssetManager();
	}

	AssetManager::AssetManager()
		: m_Resources(), m_Fonts(this), m_Textures(this), m_Materials(this), m_Models(this)
	{
	
	}

	const MaterialManager& AssetManager::Materials() const
	{
		return m_Materials;
	}

	const FontManager& AssetManager::Fonts() const
	{
		return m_Fonts;
	}

	const TextureManager& AssetManager::Textures() const
	{
		return m_Textures;
	}

	const BasicModels& AssetManager::Meshes() const
	{
		return m_Models;
	}

	void AssetManager::LoadPack(const FilePath& resourcePack, std::function<void(const ResourcePack&)> callback)
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
			result.m_Resources[resFile.Name] = std::move(resFile);
		}
		for (auto& pair : result.m_Resources)
		{
			LoadFile(pair.second); ;
		}
		callback(result);
	}

	bool AssetManager::ResourceExists(const ResourceID& id)
	{
		return m_Resources.find(id) != m_Resources.end();
	}

	id_t AssetManager::RegisterGetId(std::unique_ptr<Resource>&& resource)
	{
		id_t id = FindNextId();
		m_Resources[id] = std::move(resource);
		return id;
	}

	AssetHandle<Resource> AssetManager::GetResource(const ResourceID& id)
	{
		if (ResourceExists(id))
		{
			return AssetHandle<Resource>(m_Resources.at(id).get(), false);
		}
		return nullptr;
	}

	void AssetManager::FreeResource(const ResourceID& id)
	{
		Resource* resource = GetResource(id).Get();
		m_Resources.erase(id);
	}

	id_t AssetManager::FindNextId()
	{
		id_t id = 0;
		while (ResourceExists(id))
		{
			id++;
		}
		return id;
	}

	ResourceType AssetManager::StringToType(const std::string& str)
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

	void AssetManager::LoadFile(ResourceFile& resourceFile)
	{
		switch (resourceFile.Type)
		{
		case ResourceType::Texture2D:
			return LoadTexture2DFile(resourceFile);
		case ResourceType::Model:
			return LoadModelFile(resourceFile);
		default:
			break;
		}
		BLT_ASSERT(false, "Unable to load Resource File " + resourceFile.Name);
	}

	void AssetManager::LoadTexture2DFile(ResourceFile& resourceFile)
	{
		int width = std::stoi(resourceFile.Attributes.GetChild("width").Data.c_str());
		int height = std::stoi(resourceFile.Attributes.GetChild("height").Data.c_str());
		resourceFile.Id = RegisterGetId(std::make_unique<Texture2D>(width, height));
		Texture2D* ptr = (Texture2D*)m_Resources[resourceFile.Id].get();
		std::string data = resourceFile.Attributes.GetChild("data").Data;
		const std::string& magString = resourceFile.Attributes.GetChild("options").Attributes.at("magnification");
		const std::string& minString = resourceFile.Attributes.GetChild("options").Attributes.at("minification");
		const std::string& mipmapString = resourceFile.Attributes.GetChild("options").Attributes.at("mipmaps");
		const std::string& wrapString = resourceFile.Attributes.GetChild("options").Attributes.at("wrap");
		BLT_ASSERT(magString == "Nearest" || magString == "Linear", "Invalid Mag Option");
		BLT_ASSERT(minString == "Nearest" || minString == "Linear", "Invalid Min Option");
		BLT_ASSERT(mipmapString == "Enabled" || mipmapString == "Disabled", "Invalid Mipmap Option");
		BLT_ASSERT(wrapString == "Repeat" || wrapString == "ClampToEdge", "Invalid Wrap Option");
		Image image;
		image.Width = width;
		image.Height = height;
		image.Components = 4;
		image.Pixels = BLT_NEW byte[data.length()];
		memcpy(image.Pixels, data.data(), data.length());
		TextureCreateOptions options;
		options.Magnification = (magString == "Nearest") ? MagFilter::Nearest : MagFilter::Linear;
		options.Minification = (minString == "Nearest") ? MinFilter::Nearest : MinFilter::Linear;
		options.MipmapMode = (mipmapString == "Disabled") ? Mipmaps::Disabled : Mipmaps::Enabled;
		options.Wrap = (wrapString == "Repeat") ? WrapMode::Repeat : WrapMode::ClampToEdge;
		*ptr = Texture2D(std::move(image), options);
	}

	void AssetManager::LoadModelFile(ResourceFile& resourceFile)
	{
		resourceFile.Id = RegisterGetId(std::make_unique<Mesh>(MeshData()));
		Mesh* ptr = (Mesh*)m_Resources[resourceFile.Id].get();
		Task t = TaskManager::Get().Run([resourceFile{ std::move(resourceFile) }]()
			{
				int vertexDimension = std::stoi(resourceFile.Attributes.GetChild("vertices").Attributes.at("dim").c_str());
				std::vector<std::string_view> verticesS = blt::split_view(resourceFile.Attributes.GetChild("vertices").Data, ' ');
				std::vector<std::string_view> normalsS = blt::split_view(resourceFile.Attributes.GetChild("normals").Data, ' ');
				std::vector<std::string_view> texcoordsS = blt::split_view(resourceFile.Attributes.GetChild("uvs").Data, ' ');
				std::vector<std::string_view> indicesS = blt::split_view(resourceFile.Attributes.GetChild("indices").Data, ' ');
				std::vector<float> vertices;
				std::vector<float> normals;
				std::vector<float> texcoords;
				std::vector<uint32_t> indices;
				vertices.resize(verticesS.size());
				normals.resize(normalsS.size());
				texcoords.resize(texcoordsS.size());
				indices.resize(indicesS.size());
				std::transform(verticesS.begin(), verticesS.end(), vertices.begin(), [](const std::string_view& str) {
					return std::stof(str.data());
					});
				std::transform(normalsS.begin(), normalsS.end(), normals.begin(), [](const std::string_view& str) {
					return std::stof(str.data());
					});
				std::transform(texcoordsS.begin(), texcoordsS.end(), texcoords.begin(), [](const std::string_view& str) {
					return std::stof(str.data());
					});
				std::transform(indicesS.begin(), indicesS.end(), indices.begin(), [](const std::string_view& str) {
					return (uint32_t)std::stoi(str.data());
					});
				return std::tuple<std::vector<float>, std::vector<float>, std::vector<float>, std::vector<uint32_t>>{ std::move(vertices), std::move(normals), std::move(texcoords), std::move(indices) };
			});
		t.ContinueWithOnMainThread([ptr](std::tuple<std::vector<float>, std::vector<float>, std::vector<float>, std::vector<uint32_t>> result)
			{
				std::vector<float>& vertices = std::get<0>(result);
				std::vector<float>& normals = std::get<1>(result);
				std::vector<float>& texcoords = std::get<2>(result);
				std::vector<uint32_t>& indices = std::get<3>(result);
				int vertexDimension = 3;
				MeshData data;
				data.Indices = std::make_unique<IndexArray>();
				data.Indices->AddIndexBuffer(std::make_unique<IndexBuffer>(indices.data(), indices.size()));
				data.Vertices = std::make_unique<VertexArray>();
				BufferLayout layout = BufferLayout::Default();
				VertexBuffer& buffer = data.Vertices->CreateVertexBuffer((uint32_t)vertices.size() / vertexDimension * layout.Size(), layout);
				ScopedVertexMap vertexMap = buffer.MapScoped(Access::Write);
				DefaultVertexIterator it = vertexMap.DefaultBegin();
				for (int vertex = 0; vertex < vertices.size() / vertexDimension; vertex++)
				{
					int vIndex = vertex * vertexDimension;
					int nIndex = vertex * 3;
					int tIndex = vertex * 2;
					it[0] = Vector3f(vertices[vIndex + 0], vertices[vIndex + 1], vertices[vIndex + 2]);
					it[1] = Vector3f(normals[nIndex + 0], normals[nIndex + 1], normals[nIndex + 2]);
					it[2] = Vector2f(texcoords[tIndex + 0], texcoords[tIndex + 1]);
					it[3] = Color::White.ToBytes();
					it++;
				}
				ptr->Data() = std::move(data);
			});		
	}

}