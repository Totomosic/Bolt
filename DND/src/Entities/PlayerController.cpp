#include "bltpch.h"
#include "PlayerController.h"

namespace DND
{

	PlayerController::PlayerController() : Component(),
		m_Left(Keycode::A), m_Up(Keycode::W), m_Right(Keycode::D), m_Down(Keycode::S)
	{
	
	}

	void PlayerController::Update()
	{
		TileTransform& t = gameObject()->Components().GetComponent<TileTransform>();
		TileMotion& m = gameObject()->Components().GetComponent<TileMotion>();
		if (!m.IsMoving())
		{
			if (Input::KeyDown(m_Up))
			{
				m.SetTargetTile(t.CurrentTile() + Tile(0, 1));
			}
			else if (Input::KeyDown(m_Down))
			{
				m.SetTargetTile(t.CurrentTile() + Tile(0, -1));
			}
			else if (Input::KeyDown(m_Left))
			{
				m.SetTargetTile(t.CurrentTile() + Tile(-1, 0));
			}
			else if (Input::KeyDown(m_Right))
			{
				m.SetTargetTile(t.CurrentTile() + Tile(1, 0));
			}
		}
	}

	std::unique_ptr<Component> PlayerController::Clone() const
	{
		return std::unique_ptr<PlayerController>();
	}

}