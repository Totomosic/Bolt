#include "TileMapLevel.h"
#include "..\Entities\PlayerWizard.h"

namespace RPG
{

	TileMapLevel::TileMapLevel(int width, int height, Vector2i tileSize, id_t layerId)
		: m_Width(width), m_Height(height), m_TileSize(tileSize), m_Tiles(), m_Layer(layerId)
	{
		m_Tiles.resize(m_Width * m_Height);
	}

	TileMapLevel::TileMapLevel(const Image& level, Vector2i tileSize, id_t layerId) : TileMapLevel(level.Width, level.Height, tileSize, layerId)
	{
		for (int y = 0; y < m_Height; y++)
		{
			for (int x = 0; x < m_Width; x++)
			{
				byte* pixel = level.GetPixel(x, y);
				Color c(pixel[0], pixel[1], pixel[2], pixel[3]);
				TileType type = TileType::Grass;
				if (c == Color(0, 255, 0))
				{
					type = TileType::Grass;
				}
				else if (c == Color(255, 255, 0))
				{
					type = TileType::Sand;
				}
				else if (c == Color(0, 0, 255))
				{
					type = TileType::Water;
				}
				else if (c == Color(127, 127, 127))
				{
					type = TileType::StonePath;
				}
				CreateTile(x, y, type);
			}
		}
	}

	int TileMapLevel::Width() const
	{
		return m_Width;
	}

	int TileMapLevel::Height() const
	{
		return m_Height;
	}

	const Vector2i& TileMapLevel::TileSize() const
	{
		return m_TileSize;
	}

	const std::vector<std::unique_ptr<MapTile>>& TileMapLevel::Tiles() const
	{
		return m_Tiles;
	}

	const MapTile& TileMapLevel::GetTile(int x, int y) const
	{
		if (x < 0 || x >= m_Width || y < 0 || y >= m_Height)
		{
			return *(MapTile*)nullptr;
		}
		return *m_Tiles[x + y * m_Width];
	}

	MapTile& TileMapLevel::GetTile(int x, int y)
	{
		if (x < 0 || x >= m_Width || y < 0 || y >= m_Height)
		{
			return *(MapTile*)nullptr;
		}
		return *m_Tiles[x + y * m_Width];
	}

	NPCcharacter* TileMapLevel::GetNPC(int index) const
	{
		return m_NPCs.at(index).get();
	}

	NPCcharacter* TileMapLevel::AddNPC(std::unique_ptr<NPCcharacter>&& npc, bool create)
	{
		NPCcharacter* ptr = npc.get();
		m_NPCs.push_back(std::move(npc));
		if (create)
		{
			ptr->Create();
		}
		return ptr;
	}

	void TileMapLevel::RemoveNPC(int index)
	{
		NPCcharacter* character = GetNPC(index);
		if (character->IsCreated())
		{
			character->Destroy();
		}
		m_NPCs.erase(m_NPCs.begin() + index);
	}

	void TileMapLevel::RemoveNPC(NPCcharacter* character)
	{
		if (character->IsCreated())
		{
			character->Destroy();
		}
		auto it = std::find_if(m_NPCs.begin(), m_NPCs.end(), [character](const std::unique_ptr<NPCcharacter>& chr)
		{
			return chr.get() == character;
		});
		if (it != m_NPCs.end())
		{
			m_NPCs.erase(it);
		}
	}

	MapTile& TileMapLevel::CreateTile(int x, int y, TileType type)
	{
		m_Tiles[x + y * m_Width] = std::make_unique<MapTile>(Vector2i(x, y), m_TileSize, type, m_Layer);
		MapTile& tile = GetTile(x, y);
		if (type == TileType::Water)
		{
			tile.Disable();
		}
		return tile;
	}

	void TileMapLevel::Update(const PlayerWizard* player)
	{
		std::vector<NPCcharacter*> requireDelete;
		for (std::unique_ptr<NPCcharacter>& character : m_NPCs)
		{
			if (character->IsCreated())
			{
				character->Update();
				character->NPCUpdate(player);
			}
		}
		for (NPCcharacter* character : requireDelete)
		{
			RemoveNPC(character);
		}
	}

	void TileMapLevel::Create(int x, int y, int width, int height)
	{
		for (int i = x; i < x + width; i++)
		{
			for (int j = y; j < y + height; j++)
			{
				MapTile* tile = &GetTile(i, j);
				if (tile != nullptr && !tile->IsCreated())
				{
					tile->Create();
				}
			}
		}
	}

	void TileMapLevel::Create()
	{
		for (std::unique_ptr<MapTile>& tile : m_Tiles)
		{
			if (!tile->IsCreated())
			{
				tile->Create();
			}
		}
	}

	void TileMapLevel::Destroy(int x, int y, int width, int height)
	{
		for (int i = x; i < x + width; i++)
		{
			for (int j = y; j < y + height; j++)
			{
				MapTile* tile = &GetTile(i, j);
				if (tile != nullptr && tile->IsCreated())
				{
					tile->Destroy();
				}
			}
		}
	}

	void TileMapLevel::Destroy()
	{
		for (std::unique_ptr<MapTile>& tile : m_Tiles)
		{
			if (tile->IsCreated())
			{
				tile->Destroy();
			}
		}
	}

	void TileMapLevel::CreateNPCs()
	{
		for (std::unique_ptr<NPCcharacter>& character : m_NPCs)
		{
			if (!character->IsCreated())
			{
				character->Create();
			}
		}
	}

	void TileMapLevel::DestroyNPCs()
	{
		for (std::unique_ptr<NPCcharacter>& character : m_NPCs)
		{
			if (character->IsCreated())
			{
				character->Destroy();
			}
		}
	}

}