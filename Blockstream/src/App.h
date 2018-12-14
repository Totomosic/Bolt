#pragma once
#include "BoltInclude.h"

#include "MapTile.h"
#include "MapTileManager.h"
#include "GameManager.h"

namespace Blockstream
{

	class BlockstreamClient : public Application
	{
	public:
		const float ROTATE_X_SENSE = 0.005f;
		const float ROTATE_Y_SENSE = 0.005f;
		const float CAMERA_ZOOM_SPEED = 10;

		const float TILE_WIDTH = 20;
		const float TILE_HEIGHT = 20;
		const float TILE_DEPTH = 80;
		const int X_TILES = 9;
		const int Y_TILES = 9;
		const int WATER_STRENGTH = 3;

	public:
		bool isPlaying;
		bool isTypingHost;
		blt::string hostString;

		id_t backgroundTitleTexture;
		id_t backgroundSpaceTexture;

		Scene* scene;
		Layer* mainLayer;
		Layer* uiLayer;

		GameObject* mainCameraAnchor;
		Camera* mainCamera;
		Camera* uiCamera;

		ObjectFactory factory;
		id_t tileMapPrefabId;

		Text* fpsText;
		Text* hostStringText;

	public:
		~BlockstreamClient() override;
		void Init() override;
		void Tick() override;
		void Update() override;
		void Render() override;

		void ClearScreen();
		void CreateTitleScreen();
		void CreateHostScreen();
		void CreateJoinScreen();
		void CreateGameScreen();
		void CreateEndScreen(const blt::string& text);

	};

}