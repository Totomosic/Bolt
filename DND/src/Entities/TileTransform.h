#pragma once
#include "bltpch.h"
#include "../Map/TilemapManager.h"
#include "TileMapTracker.h"

namespace DND
{

	class TileTransform : public Component
	{
	private:
		Tile m_CurrentTile;
		Vector3f m_PositionOffset;

	public:
		TileTransform(Tile currentTile = Tile(), Vector3f positionOffset = Vector3f());

		const Tile& CurrentTile() const;
		const Vector3f& PositionOffset() const;
		Vector3f PositionOfTile() const;

		void Start() override;
		void SetCurrentTile(Tile tile, bool updatePosition);
		void SetPositionOffset(Vector3f positionOffset);

		std::unique_ptr<Component> Clone() const override;

	private:
		void UpdatePosition() const;

	};

}