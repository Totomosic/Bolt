#include "bltpch.h"
#include "NPCController.h"

namespace DND
{

	NPCController::NPCController() : Component()
	{

	}

	void NPCController::Update()
	{
		TileTransform& t = gameObject()->Components().GetComponent<TileTransform>();
		TileMotion& m = gameObject()->Components().GetComponent<TileMotion>();
		if (!m.IsMoving())
		{
			float decision = Random::NextFloat(0, 1000);
			if (decision <= 1)
			{
				int direction = Random::NextInt(0, 1);
				int dir = Random::NextInt(0, 1);
				Tile diff;
				if (direction == 0)
				{
					diff.x = (dir == 0) ? -1 : 1;
				}
				else
				{
					diff.y = (dir == 0) ? -1 : 1;
				}
				m.SetTargetTile(t.CurrentTile() + diff);
			}
		}
	}

	std::unique_ptr<Component> NPCController::Clone() const
	{
		return std::unique_ptr<NPCController>();
	}

}