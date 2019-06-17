#include "mcpch.h"
#include "ChunkManager.h"

namespace Minecraft
{

	ChunkManager::ChunkManager(ObjectFactory& factory, const ResourcePtr<Texture2D>& atlas, int visibleXChunks, int visibleZChunks)
		: m_VisibleXChunks(visibleXChunks), m_VisibleZChunks(visibleZChunks), m_VisibleRegion(visibleXChunks, visibleZChunks), m_LoadedChunks(), m_ChunkObjects(nullptr)
	{
		BLT_ASSERT(visibleXChunks > 0 && visibleZChunks > 0, "invalid dimension");
		m_ChunkObjects = std::make_unique<ChunkObject[]>(m_VisibleXChunks * m_VisibleZChunks);
		for (int x = 0; x < visibleXChunks; x++)
		{
			for (int z = 0; z < visibleZChunks; z++)
			{
				auto chunk = std::make_unique<ChunkColumnData>(16, 16, 16, 16);
				m_VisibleRegion.SetChunk(x, z, chunk.get());
				ResourcePtr<Model> chunkModel = ResourceManager::Get().Register(std::make_unique<Model>(ModelData()));
				auto material = ResourceManager::Get().Materials().TextureLighting();
				material->GetLinkContext().Link("Texture", atlas);
				Mesh m;
				m.Models.push_back({ chunkModel });
				m.Materials.push_back(std::move(material));
				GameObject* chunkObject = factory.Instantiate(m, Transform({ (float)x * chunk->GetWidthInBlocks(), 0, (float)z * chunk->GetHeightInBlocks() }));
				m_ChunkObjects[x + z * m_VisibleXChunks] = { chunkModel, chunkObject };
				m_LoadedChunks.push_back({ {x, z}, std::move(chunk) });
			}
		}
	}

	int ChunkManager::GetVisibleXChunks() const
	{
		return m_VisibleXChunks;
	}

	int ChunkManager::GetVisibleZChunks() const
	{
		return m_VisibleZChunks;
	}

	const ChunkRegion& ChunkManager::GetChunkRegion() const
	{
		return m_VisibleRegion;
	}

	ChunkRegion& ChunkManager::GetChunkRegion()
	{
		return m_VisibleRegion;
	}

	void ChunkManager::BuildChunk(int x, int z) const
	{
		Model* modelPtr = m_ChunkObjects[x + z * GetVisibleXChunks()].ModelPtr.Get();
		TaskManager::Run([x, z, this]()
			{
				return GetChunkRegion().GetFaces(x, z);
			}).ContinueWithOnMainThread([modelPtr](std::vector<BlockFace> faces)
				{
					ModelData data;
					data.Vertices = std::make_unique<VertexArray>();
					data.Indices = std::make_unique<IndexArray>();
					data.Indices->AddIndexBuffer(std::make_unique<IndexBuffer>(faces.size() * 6));
					BufferLayout layout = BufferLayout::Default();
					VertexBuffer& buffer = data.Vertices->CreateVertexBuffer(faces.size() * 4 * layout.Size(), layout);
					Vector4<byte> color = Color::White.ToBytes();
					if (faces.size() > 0)
					{
						VertexMapping vMapping = data.Vertices->Map();
						IndexMapping iMapping = data.Indices->Map();
						VertexIterator it = vMapping.Begin();
						IndexIterator indices = iMapping.Begin();
						for (int i = 0; i < faces.size(); i++)
						{
							BlockFace& face = faces.at(i);
							it[0] = face.TopLeft;
							it[1] = face.Normal;
							it[2] = Vector2f(face.Texture.Min.x, face.Texture.Max.y);
							it[3] = color;
							it++;
							it[0] = face.BottomLeft;
							it[1] = face.Normal;
							it[2] = face.Texture.Min;
							it[3] = color;
							it++;
							it[0] = face.BottomRight;
							it[1] = face.Normal;
							it[2] = Vector2f(face.Texture.Max.x, face.Texture.Min.y);
							it[3] = color;
							it++;
							it[0] = face.TopRight;
							it[1] = face.Normal;
							it[2] = face.Texture.Max;
							it[3] = color;
							it++;
							*indices = (uint32_t)i * 4 + 0;
							indices++;
							*indices = (uint32_t)i * 4 + 1;
							indices++;
							*indices = (uint32_t)i * 4 + 2;
							indices++;
							*indices = (uint32_t)i * 4 + 0;
							indices++;
							*indices = (uint32_t)i * 4 + 2;
							indices++;
							*indices = (uint32_t)i * 4 + 3;
							indices++;
						}
					}
					modelPtr->Data() = std::move(data);
				});
	}

	void ChunkManager::BuildAllChunks() const
	{
		for (int x = 0; x < m_VisibleXChunks; x++)
		{
			for (int z = 0; z < m_VisibleZChunks; z++)
			{
				BuildChunk(x, z);
			}
		}
	}

}