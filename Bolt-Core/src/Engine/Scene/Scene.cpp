#include "Scene.h"
#include "SceneManager.h"

namespace Bolt
{

	Scene::Scene()
		: m_Layers(), m_LayerNames(), m_Cameras()
	{
		ClearCameras();
	}

	const Layer& Scene::GetLayer(id_t id) const
	{
		return m_Layers[id];
	}

	Layer& Scene::GetLayer(id_t id)
	{
		return m_Layers[id];
	}

	const Layer& Scene::GetLayer(const blt::string& layerName) const
	{
		return GetLayer(GetIdOfLayer(layerName));
	}

	Layer& Scene::GetLayer(const blt::string& layerName)
	{
		return GetLayer(GetIdOfLayer(layerName));
	}

	id_t Scene::GetIdOfLayer(const blt::string& name) const
	{
		return m_LayerNames.at(name);
	}

	id_t Scene::GetMaskOfLayer(const blt::string& name) const
	{
		return GetMaskOfLayer(GetIdOfLayer(name));
	}

	id_t Scene::GetMaskOfLayer(id_t id) const
	{
		return BLT_BIT(id);
	}

	id_t Scene::GetMaskOfLayers(const std::vector<blt::string>& layers) const
	{
		id_t mask = 0;
		for (const blt::string& name : layers)
		{
			mask |= GetMaskOfLayer(name);
		}
		return mask;
	}

	id_t Scene::GetMaskOfLayers(const std::vector<id_t>& layers) const
	{
		id_t mask = 0;
		for (id_t id : layers)
		{
			mask |= GetMaskOfLayer(id);
		}
		return mask;
	}

	const Camera* Scene::GetCameraById(id_t id) const
	{
		return &m_Cameras[id];
	}

	Camera* Scene::GetCameraById(id_t id)
	{
		return &m_Cameras[id];
	}

	std::vector<const Layer*> Scene::GetAllLayers() const
	{
		std::vector<const Layer*> result;
		for (int i = 0; i < MAX_LAYERS; i++)
		{
			if (m_Layers[i].m_Id != GameObject::InvalidID)
			{
				result.push_back(&m_Layers[i]);
			}
		}
		return result;
	}

	std::vector<const Layer*> Scene::GetLayers(id_t mask) const
	{
		std::vector<const Layer*> result;
		id_t maxLayer = (int)log2(mask);
		for (id_t i = 0; i <= maxLayer; i++)
		{
			if (BLT_IS_BIT_SET(mask, i))
			{
				result.push_back(&GetLayer(i));
			}
		}
		return result;
	}

	Layer* Scene::CreateLayer(const blt::string& name, std::unique_ptr<SceneGraph>&& sceneGraph)
	{
		id_t id = FindNextId();
		m_Layers[id].m_Id = id;
		m_Layers[id].Enable();
		m_Layers[id].m_SceneGraph = std::move(sceneGraph);
		m_LayerNames[name] = id;
		return &m_Layers[id];
	}

	Camera* Scene::CreateCamera(const Projection& projection)
	{
		id_t id = FindNextCameraId();
		Camera* camera = &m_Cameras[id];
		camera->SetProjection(projection);
		camera->SetID(id);
		return camera;
	}

	Camera* Scene::CreateCamera(const Frustum& frustum, ProjectionType type)
	{
		return CreateCamera({ frustum, type, (type == ProjectionType::Orthographic) ? Matrix4f::Orthographic(frustum) : Matrix4f::Perspective(frustum) });
	}

	void Scene::RemoveCamera(id_t id)
	{
		m_Cameras[id].SetID(GameObject::InvalidID);
	}

	void Scene::RemoveCamera(const Camera* camera)
	{
		RemoveCamera(camera->ID());
	}

	void Scene::Update()
	{
		for (id_t i = 0; i < MAX_LAYERS; i++)
		{
			if (m_Layers[i].IsEnabled())
			{
				m_Layers[i].Update();
			}
		}
		for (id_t i = 0; i < MAX_CAMERAS; i++)
		{
			if (m_Cameras[i].ID() != GameObject::InvalidID)
			{
				m_Cameras[i].Update();
				m_Cameras[i].LateUpdate();
			}
		}
	}

	void Scene::UpdateTemporaryObjects()
	{
		for (int i = 0; i < MAX_LAYERS; i++)
		{
			if (m_Layers[i].IsEnabled())
			{
				m_Layers[i].UpdateTemporaryObjects();
			}
		}
	}

	void Scene::ClearCameras()
	{
		for (id_t i = 0; i < Scene::MAX_CAMERAS; i++)
		{
			m_Cameras[i].m_Id = GameObject::InvalidID;
		}
	}

	id_t Scene::FindNextId() const
	{
		for (id_t i = 0; i < Scene::MAX_LAYERS; i++)
		{
			if (!m_Layers[i].IsEnabled())
			{
				return i;
			}
		}
		BLT_ASSERT(false, "Unable to find valid Id for Layer");
		return GameObject::InvalidID;
	}

	id_t Scene::FindNextCameraId() const
	{
		for (id_t i = 0; i < Scene::MAX_CAMERAS; i++)
		{
			if (m_Cameras[i].ID() == GameObject::InvalidID)
			{
				return i;
			}
		}
		BLT_ASSERT(false, "Unable to find valid Id for Camera");
		return GameObject::InvalidID;
	}

}