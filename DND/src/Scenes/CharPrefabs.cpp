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

		ResourcePtr<Texture2D> downSwordsman = ResourceManager::Get<Texture2D>(resources.GetResourceId("SwordsmanStaticForward"));
		ResourcePtr<Texture2D> rightSwordsman = ResourceManager::Get<Texture2D>(resources.GetResourceId("SwordsmanStaticRight"));
		ResourcePtr<Texture2D> upSwordsman = ResourceManager::Get<Texture2D>(resources.GetResourceId("SwordsmanStaticBackward"));
		ResourcePtr<Texture2D> leftSwordsman = ResourceManager::Get<Texture2D>(resources.GetResourceId("SwordsmanStaticLeft"));

		CharacterStats defaultStats;
		defaultStats.MaxHealth = 40;
		defaultStats.CurrentHealth = defaultStats.MaxHealth;

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
		wizardCharacterPrefab.Components().AddComponent<Healthbar>(factory.CurrentLayer(), Vector3f(0, TILE_HEIGHT * 0.75f + 5, 1));

		Mesh swordsmanCharacterMesh = wizardCharacterMesh;

		ObjectPrefab swordsmanCharacterPrefab;
		swordsmanCharacterPrefab.Components().AddComponent<MeshRenderer>(swordsmanCharacterMesh);
		swordsmanCharacterPrefab.Components().AddComponent<TileTransform>(&GameManager::Get().GetTilemap(), Tile(), Vector3f(0, 15, 0));
		swordsmanCharacterPrefab.Components().AddComponent<CharacterAnimator>(Direction::Down, upSwordsman, rightSwordsman, downSwordsman, leftSwordsman);
		swordsmanCharacterPrefab.Components().AddComponent<TileMotion>(350);
		swordsmanCharacterPrefab.Components().AddComponent<SpellCaster>();
		swordsmanCharacterPrefab.Components().AddComponent<StatsComponent>(defaultStats);
		swordsmanCharacterPrefab.Components().AddComponent<Healthbar>(factory.CurrentLayer(), Vector3f(0, TILE_HEIGHT * 0.75f + 5, 1));

		prefabs.BlueWizard = GameManager::Get().Network().Factory().AddPrefab(std::move(wizardCharacterPrefab));
		prefabs.Swordsman = GameManager::Get().Network().Factory().AddPrefab(std::move(swordsmanCharacterPrefab));
	}

}