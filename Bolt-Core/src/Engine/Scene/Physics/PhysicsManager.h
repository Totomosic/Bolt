#pragma once
#include "PhysicsEngine.h"

#ifndef LAYERS_PER_SCENE
#define LAYERS_PER_SCENE 12
#endif

namespace Bolt
{

	struct Scene;

	class BLT_API PhysicsManager
	{
	public:
		struct BLT_API EngineInfo
		{
		public:
			PhysicsEngine Engine;
			bool InUse;
		};

		static constexpr uint32_t MAX_PHYSICS_ENGINES = LAYERS_PER_SCENE;

	private:
		Scene* m_Scene;
		EngineInfo m_Engines[MAX_PHYSICS_ENGINES];
		std::unordered_map<id_t, id_t> m_LayerMaskMap;

	public:
		PhysicsManager(Scene* scene);

		// Returns the physics engine with given Id
		const PhysicsEngine& GetEngineById(id_t engineId) const;
		// Returns the physics engine with given Id
		PhysicsEngine& GetEngineById(id_t engineId);

		// Returns the Id of physics engine responsible for handling specified layer
		id_t GetIdOfEngine(id_t layerId) const;
		// Creates a physics engine that handles objects from all layers specified in layerMask, returns PhysicsEngine Id
		// Layers that have already been merged cannot be remerged unless adding a layer
		// eg. After merging layers 0 and 1, layer 1 cannot be merged with layer 2
		// layer 2 can, however, be merged with layers 0 and 1
		id_t MergeLayers(id_t layerMask);

		// Adds GameObject to correct PhysicsEngine, returns Id of PhysicsEngine
		id_t AddGameObject(GameObject* object);
		// Removes GameObject from correct PhysicsEngine, returns Id of PhysicsEngine
		id_t RemoveGameObject(GameObject* object);

		// Updates all physics Engines
		void Update(double elapsedTime);

	private:
		// Resets to each layer being individual
		void ResetMaskMap();

	};

}