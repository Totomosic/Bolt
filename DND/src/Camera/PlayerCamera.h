#pragma once
#include "bltpch.h"
#include "../Map/Tilemap.h"

namespace DND
{

	class PlayerCamera : public Component
	{
	private:
		Tilemap* m_Tilemap;
		GameObject* m_Player;
		GameObject* m_TileSelector;
		Tile m_SelectedTile;
		Layer* m_Layer;

	public:
		PlayerCamera(Tilemap* tilemap, GameObject* player, Layer* selectorLayer);

		GameObject* Player() const;
		GameObject* TileSelector() const;
		const Tile& SelectedTile() const;

		void Start() override;
		void Update() override;

		void SetPlayer(GameObject* player);
		void SetTileSelector(GameObject* selector);
		void HideTileSelector();
		void ShowTileSelector();

		std::unique_ptr<Component> Clone() const override;

	};

}