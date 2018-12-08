#pragma once
#include "BoltInclude.h"
#include "Events.h"

namespace Blockstream
{

	const float HEIGHT_PER_LEVEL = 20;
	const float TILE_SPEED = 100;

	class MapTile : public Component
	{
	private:
		int m_Level;
		int m_TargetLevel;
		bool m_Water;
		bool m_End;

	public:
		MapTile(int level = 0);

		void Start() override;
		void Update() override;

		int GetLevel() const;
		bool IsFlooded() const;

		void SetLevel(int level);
		void IncrementLevel();
		void DecrementLevel();
		void SetWater(bool flooded);
		void MakeEndTile();

		std::unique_ptr<Component> Clone() const override;

	};

}