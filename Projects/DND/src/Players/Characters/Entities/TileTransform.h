#pragma once
#include "CurrentMap.h"

namespace DND
{

	class TileTransform : public Component
	{
	private:
		CurrentMap* m_CurrentMap;

		Tile m_BottomLeftTile;
		int m_Width;
		int m_Height;

	public:
		TileTransform(const Tile& bottomLeftTile);
		TileTransform(const Tile& bottomLeftTile, int width, int height);

		const Tile& BottomLeftTile() const;
		Tile TopRightTile() const;
		int Width() const;
		int Height() const;
		Vector3f WorldPositionOfBottomLeft() const;
		Vector3f WorldPositionOfTopRight() const;
		Vector3f WorldPositionOfCenter() const;
		Vector3f WorldPositionOfTile(const Tile& tile) const;
		bool IsTileInMap(const Tile& tile) const;

		void Start() override;
		void SetBottomLeftTile(const Tile& bottomLeftTile, bool updatePosition = false);
		void SetWidth(int width, bool updatePosition = false);
		void SetHeight(int height, bool updatePosition = false);
		void SetSize(const Vector2i& size, bool updatePosition = false);

		void UpdateWorldPosition();

		std::unique_ptr<Component> Clone() const override;

	};

}