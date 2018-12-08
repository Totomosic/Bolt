#pragma once
#include "Layer.h"
#include "ObjectCollection.h"

#ifndef LAYERS_PER_SCENE
#define LAYERS_PER_SCENE 12
#endif

namespace Bolt
{

	struct BLT_API Scene
	{
	public:
		static constexpr int MAX_LAYERS = LAYERS_PER_SCENE;
		static constexpr int MAX_CAMERAS = 32;

	private:
		Layer m_Layers[MAX_LAYERS];
		std::unordered_map<blt::string, int> m_LayerNames;
		Camera m_Cameras[MAX_CAMERAS];

	public:
		Scene();

		const Layer& GetLayer(id_t id) const;
		Layer& GetLayer(id_t id);
		const Layer& GetLayer(const blt::string& name) const;
		Layer& GetLayer(const blt::string& name);
		id_t GetIdOfLayer(const blt::string& name) const;
		id_t GetMaskOfLayer(const blt::string& name) const;
		id_t GetMaskOfLayer(id_t id) const;
		id_t GetMaskOfLayers(const std::vector<blt::string>& layers) const;
		id_t GetMaskOfLayers(const std::vector<id_t>& layerIds) const;
		const Camera* GetCameraById(id_t id) const;
		Camera* GetCameraById(id_t id);

		std::vector<const Layer*> GetAllLayers() const;
		std::vector<const Layer*> GetLayers(id_t mask) const;

		Layer* CreateLayer(const blt::string& name);

		Camera* CreateCamera(const Projection& projection = Projection());
		Camera* CreateCamera(const Frustum& frustum, ProjectionType type);
		void RemoveCamera(id_t id);
		void RemoveCamera(const Camera* camera);

		void Update();
		void UpdateTemporaryObjects();

	private:
		void ClearCameras();
		id_t FindNextId() const;
		id_t FindNextCameraId() const;

	};

}