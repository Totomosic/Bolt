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

	private:
		EventBus m_EventBus;
		std::unique_ptr<Layer[]> m_Layers;
		int m_LayerCapacity;
		Camera m_Cameras[MAX_CAMERAS];
		id_t m_Id;
		bool m_IsActive;

		PhysicsManager m_PhysEngine;

	public:
		EventEmitter<SceneLoadedEvent> OnLoad;
		EventEmitter<SceneUnloadedEvent> OnUnload;

	public:
		Scene(int layerCount = MAX_LAYERS);
		Scene(const Scene& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		const PhysicsManager& Physics() const;
		PhysicsManager& Physics();
		id_t Id() const;
		bool IsActive() const;

		const Layer& GetLayer(id_t id) const;
		Layer& GetLayer(id_t id);
		id_t GetMaskOfLayer(id_t id) const;
		id_t GetMaskOfLayers(const std::vector<id_t>& layerIds) const;
		const Camera* GetCameraById(id_t id) const;
		Camera* GetCameraById(id_t id);

		std::vector<const Layer*> GetAllLayers() const;
		std::vector<const Layer*> GetLayers(id_t mask) const;
		std::vector<Layer*> GetAllLayers();
		std::vector<Layer*> GetLayers(id_t mask);

		Layer& CreateLayer(Camera* activeCamera = nullptr, int maxGameObjects = ObjectCollection::MAX_GAMEOBJECTS);

		Camera* CreateCamera(const Projection& projection = Projection());
		Camera* CreateCamera(const Frustum& frustum, ProjectionType type);
		void RemoveCamera(id_t id);
		void RemoveCamera(const Camera* camera);

		void Update();
		void UpdateTemporaryObjects();

		SGQueryResult Query(const SGQuery& query) const;

		friend class SceneManager;

	private:
		void ClearCameras();
		id_t FindNextId() const;
		id_t FindNextCameraId() const;
		void SetIsActive(bool isActive);

	};

}