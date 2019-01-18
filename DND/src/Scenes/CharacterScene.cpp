#include "bltpch.h"
#include "CharacterScene.h"
#include "../App.h"

namespace DND
{

	void CreateCharacterScene(DndClient& client, Scene& characterScene, const ResourcePack& resources)
	{
		Projection orthoProj = Projection::Orthographic(0, client.Width(), 0, client.Height(), -100, 100);
		Camera* characterCamera = characterScene.CreateCamera(orthoProj);
		Layer& characterLayer = characterScene.CreateLayer(characterCamera);
	}

}