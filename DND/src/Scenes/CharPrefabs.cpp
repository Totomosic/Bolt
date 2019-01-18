#include "bltpch.h"
#include "../App.h"

#include "../GameManager.h"
#include "../Entities/TileMotion.h"
#include "../Entities/PlayerController.h"
#include "../Entities/Characters/CharacterStats.h"
#include "../Entities/Healthbar.h"
#include "../Camera/PlayerCamera.h"
#include "../Spells/SpellCaster.h"

namespace DND
{

	void CreateCharacterPrefabs(PrefabList& prefabs, ObjectFactory& factory, const ResourcePack& resources)
	{
		ResourcePtr<Texture2D> downWizard = ResourceManager::Get<Texture2D>(resources.GetResourceId("WizardStaticForward"));
		ResourcePtr<Texture2D> rightWizard = ResourceManager::Get<Texture2D>(resources.GetResourceId("WizardStaticRight"));
		ResourcePtr<Texture2D> upWizard = ResourceManager::Get<Texture2D>(resources.GetResourceId("WizardStaticBackward"));
		ResourcePtr<Texture2D> leftWizard = ResourceManager::Get<Texture2D>(resources.GetResourceId("WizardStaticLeft"));

		CharacterStats defaultStats;
		defaultStats.MaxHealth = 100;
		defaultStats.CurrentHealth = 100;

		ResourcePtr<Model> playerModel = ResourceManager::Get<Model>(ResourceManager::Register(std::make_unique<Model>(RectangleFactory(TILE_WIDTH * 0.9f, TILE_HEIGHT * 1.5f))));
		Mesh wizardCharacterMesh;
		wizardCharacterMesh.Models.push_back({ playerModel });
		wizardCharacterMesh.Materials[0].Shader = Shader::DefaultTexture();

		ObjectPrefab wizardCharacterPrefab;
		wizardCharacterPrefab.Components().AddComponent<MeshRenderer>(wizardCharacterMesh);
		wizardCharacterPrefab.Components().AddComponent<TileTransform>(&GameManager::Get().GetTilemap(), Tile(), Vector3f(0, 15, 0));
		wizardCharacterPrefab.Components().AddComponent<CharacterAnimator>(Direction::Down, upWizard, rightWizard, downWizard, leftWizard);
		wizardCharacterPrefab.Components().AddComponent<TileMotion>(300);
		wizardCharacterPrefab.Components().AddComponent<SpellCaster>();
		wizardCharacterPrefab.Components().AddComponent<StatsComponent>(defaultStats);
		wizardCharacterPrefab.Components().AddComponent<Healthbar>(factory.CurrentLayer(), Vector3f(0, TILE_HEIGHT * 0.75f + 5, 0));

		prefabs.BlueWizard = GameManager::Get().Network().Factory().AddPrefab(std::move(wizardCharacterPrefab));
	}

}