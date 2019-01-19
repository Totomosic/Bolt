#include "bltpch.h"
#include "PlayerCamera.h"
#include "../GameManager.h"

namespace DND
{

	PlayerCamera::PlayerCamera(Tilemap* tilemap, GameObject* player, Layer* selectorLayer) : Component(),
		m_Tilemap(tilemap), m_Player(player), m_TileSelector(nullptr), m_SelectedTile(), m_Layer(selectorLayer)
	{
	
	}

	GameObject* PlayerCamera::Player() const
	{
		return m_Player;
	}

	GameObject* PlayerCamera::TileSelector() const
	{
		return m_TileSelector;
	}

	const Tile& PlayerCamera::SelectedTile() const
	{
		return m_SelectedTile;
	}

	void PlayerCamera::Start()
	{
		ObjectFactory factory(*m_Layer);
		SetTileSelector(factory.Rectangle(TILE_WIDTH, TILE_HEIGHT, Color(200, 10, 10, 180)));
	}

	void PlayerCamera::Update()
	{
		Vector3f mousePos = Input::MousePosition(camera()->ViewWidth(), camera()->ViewHeight());
		Vector3f worldPos = mousePos + camera()->transform().Position();
		Tile t = m_Tilemap->TileFromWorldPosition(worldPos.x, worldPos.y);
		if (t.x >= 0 && t.y >= 0)
		{
			m_SelectedTile = t;
			Vector3f worldTilePos = m_Tilemap->WorldPositionOfTile(t.x, t.y);
			m_TileSelector->transform().SetLocalPosition(worldTilePos + Vector3f(0, 0, 25));
		}
	}

	void PlayerCamera::SetPlayer(GameObject* player)
	{
		m_Player = player;
	}

	void PlayerCamera::SetTileSelector(GameObject* tileSelector)
	{
		if (m_TileSelector != nullptr)
		{
			Destroy(m_TileSelector);
		}
		m_TileSelector = tileSelector;
	}

	void PlayerCamera::HideTileSelector()
	{
		
	}

	void PlayerCamera::ShowTileSelector()
	{
	
	}

	std::unique_ptr<Component> PlayerCamera::Clone() const
	{
		std::unique_ptr<PlayerCamera> c = std::make_unique<PlayerCamera>(m_Tilemap, m_Player, m_Layer);
		c->SetTileSelector(m_TileSelector);
		return std::move(c);
	}

}