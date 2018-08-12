#pragma once
#include "Layer.h"

namespace Bolt
{

	struct BLT_API Scene
	{
	public:
		static constexpr int MAX_LAYERS = 24;
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

		Layer* CreateLayer(const blt::string& name, std::unique_ptr<SceneGraph>&& sceneGraph);

		template<typename SceneGraphType>
		Layer* CreateLayer(const blt::string& name)
		{
			return CreateLayer(name, std::make_unique<SceneGraphType>());
		}

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