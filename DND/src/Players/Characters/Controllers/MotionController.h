#pragma once
#include "../Entities/TileMotion.h"
#include "../Entities/NetworkComponent.h"

namespace DND
{

	class MotionController
	{
	private:
		TileMotion* m_TileMotion;
		NetworkComponent* m_Network;

	public:
		MotionController();

		void Initialize(GameObject* object);
		void MoveToBottomLeftTile(const Tile& targetTile) const;
		void JumpToBottomLeftTile(const Tile& targetTile) const;

	};

}