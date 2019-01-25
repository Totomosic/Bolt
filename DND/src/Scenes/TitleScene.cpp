#include "bltpch.h"
#include "TitleScene.h"
#include "../App.h"

namespace DND
{

	void CreateTitleScene(DndClient& client, Scene& titleScene, const ResourcePack& resources)
	{
		Projection orthoProj = Projection::Orthographic(0, client.Width(), 0, client.Height(), -100, 100);
		Camera* titleCamera = titleScene.CreateCamera(orthoProj);
		Layer& titleLayer = titleScene.CreateLayer(titleCamera);

		ResourcePtr<Texture2D> backgroundTexture = ResourceManager::Get<Texture2D>(resources.GetResourceId("dndBackground"));
		UIsurface& background = titleLayer.UI().Image(client.Width(), client.Height(), backgroundTexture, Transform({ client.Width() / 2, client.Height() / 2, -50 }));
		UIsurface& backdrop = background.Rectangle(400, 600, Color(52, 52, 52, 220), Transform({ 0, 0, 5 }));
		Text& title = backdrop.Text("Dungeons and Dragons", Color::White, Transform({ 0, 180, 5 }));

		UIsurface& playButton = backdrop.Rectangle(300, 100, Color::Green, Transform({ 0, 0, 1 }));
		playButton.Text("Play");
		playButton.EventHandler().OnClicked.Subscribe([](id_t listenerId, UIClickedEvent& e)
		{
			SceneManager::SetCurrentSceneByName("Server");
			return true;
		});
	}

}