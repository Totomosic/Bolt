#pragma once
#include "TileTransform.h"

namespace DND
{

	class TileMotion : public Component
	{
	private:
		TileTransform* m_Transform;

		// Specified in tiles/sec
		float m_Speed;
		Tile m_TargetTile;
		bool m_IsSeeking;

	public:
		TileMotion();
		// Speed in Tiles/sec
		TileMotion(float speed);

		float TilesPerSecond() const;
		float RealSpeed() const;
		const Tile& TargetTile() const;
		bool IsSeeking() const;

		void Start() override;
		void Update() override;

		void SetSpeed(float speed);
		void MoveToBottomLeftTile(const Tile& tile);
		void JumpToBottomLeftTile(const Tile& tile);

		std::unique_ptr<Component> Clone() const override;

	};

}