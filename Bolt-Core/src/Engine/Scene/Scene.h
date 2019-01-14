#pragma once
#include "Layer.h"
#include "ObjectCollection.h"
#include "Physics\PhysicsManager.h"
#include "SceneEvents.h"

namespace Bolt
{

	struct BLT_API Scene
	{
	public:
		static constexpr int MAX_LAYERS = LAYERS_PER_SCENE;
		static constexpr int MAX_CAMERAS = 32;

	// EventDispatchers work when put above members but not below, INVESTIGATE
	public:
		EventDispatcher<SceneLoadedEvent> OnLoad;
		EventDispatcher<SceneUnloadedEvent> OnUnload;

	private:
		Layer m_Layers[MAX_LAYERS];
		Camera m_Cameras[MAX_CAMERAS];
		id_t m_Id;

		PhysicsManager m_PhysEngine;

	public:
		Scene();
		Scene(const Scene& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		const PhysicsManager& Physics() const;
		PhysicsManager& Physics();
		id_t Id() const;

		const Layer& GetLayer(id_t id) const;
		Layer& GetLayer(id_t id);
		id_t GetMaskOfLayer(id_t id) const;
		id_t GetMaskOfLayers(const std::vector<id_t>& layerIds) const;
		const Camera* GetCameraById(id_t id) const;
		Camera* GetCameraById(id_t id);

		std::vector<const Layer*> GetAllLayers() const;
		std::vector<const Layer*> GetLayers(id_t mask) const;

		Layer& CreateLayer(Camera* activeCamera = nullptr);

		Camera* CreateCamera(const Projection& projection = Projection());
		Camera* CreateCamera(const Frustum& frustum, ProjectionType type);
		void RemoveCamera(id_t id);
		void RemoveCamera(const Camera* camera);

		void Update();
		void UpdateTemporaryObjects();

		friend class SceneManager;

	private:
		void ClearCameras();
		id_t FindNextId() const;
		id_t FindNextCameraId() const;

	};

}