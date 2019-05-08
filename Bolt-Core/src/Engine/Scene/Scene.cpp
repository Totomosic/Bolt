#include "Types.h"

#include "Scene.h"
#include "SceneManager.h"

namespace Bolt
{

	Scene::Scene(int layerCount)
		: OnLoad(), OnUnload(), m_Layers(std::make_unique<Layer[]>(layerCount)), m_LayerCapacity(layerCount), m_Cameras(), m_Id(GameObject::InvalidID), m_PhysEngine(this)
	{
		ClearCameras();
	}

	const PhysicsManager& Scene::Physics() const
	{
		return m_PhysEngine;
	}

	PhysicsManager& Scene::Physics()
	{
		return m_PhysEngine;
	}

	id_t Scene::Id() const
	{
		return m_Id;
	}

	const Layer& Scene::GetLayer(id_t id) const
	{
		return m_Layers[id];
	}

	Layer& Scene::GetLayer(id_t id)
	{
		return m_Layers[id];
	}

	id_t Scene::GetMaskOfLayer(id_t id) const
	{
		return BLT_BIT(id);
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
			if (m_Layers[i].m_Id != GameObject::InvalidID && m_Layers[i].IsEnabled())
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
				const Layer& layer = GetLayer(i);
				if (layer.IsEnabled())
				{
					result.push_back(&layer);
				}
			}
		}
		return result;
	}

	std::vector<Layer*> Scene::GetAllLayers()
	{
		std::vector<Layer*> result;
		for (int i = 0; i < MAX_LAYERS; i++)
		{
			if (m_Layers[i].m_Id != GameObject::InvalidID && m_Layers[i].IsEnabled())
			{
				result.push_back(&m_Layers[i]);
			}
		}
		return result;
	}

	std::vector<Layer*> Scene::GetLayers(id_t mask)
	{
		std::vector<Layer*> result;
		id_t maxLayer = (int)log2(mask);
		for (id_t i = 0; i <= maxLayer; i++)
		{
			if (BLT_IS_BIT_SET(mask, i))
			{
				Layer& layer = GetLayer(i);
				if (layer.IsEnabled())
				{
					result.push_back(&layer);
				}
			}
		}
		return result;
	}

	Layer& Scene::CreateLayer(Camera* activeCamera)
	{
		id_t id = FindNextId();
		m_Layers[id].Create(id);
		m_Layers[id].SetActiveCamera(activeCamera);
		return m_Layers[id];
	}

	Camera* Scene::CreateCamera(const Projection& projection)
	{
		id_t id = FindNextCameraId();
		Camera* camera = &m_Cameras[id];
		camera->SetProjection(projection);
		camera->SetId(id);
		camera->m_Layer = nullptr;
		return camera;
	}

	Camera* Scene::CreateCamera(const Frustum& frustum, ProjectionType type)
	{
		return CreateCamera({ frustum, type, (type == ProjectionType::Orthographic) ? Matrix4f::Orthographic(frustum) : Matrix4f::Perspective(frustum) });
	}

	void Scene::RemoveCamera(id_t id)
	{
		m_Cameras[id].SetId(GameObject::InvalidID);
	}

	void Scene::RemoveCamera(const Camera* camera)
	{
		RemoveCamera(camera->Id());
	}

	void Scene::Update()
	{
		// Update layers in reverse order
		for (int i = (int)MAX_LAYERS - 1; i >= 0; i--)
		{
			if (m_Layers[i].IsEnabled())
			{
				m_Layers[i].Update();
			}
		}
		for (id_t i = 0; i < MAX_CAMERAS; i++)
		{
			if (m_Cameras[i].Id() != GameObject::InvalidID)
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

	SGQueryResult Scene::Query(const SGQuery& query) const
	{
		SGQueryResult result;
		for (const Layer* layer : GetAllLayers())
		{
			std::vector<GameObject*> objects = query.Evaluate(layer->GameObjects(), &result.MostRelevant, &result.LeastRelevant);
			for (GameObject* object : objects)
			{
				result.GameObjects.push_back(object);
			}
		}
		return result;
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
			if (m_Cameras[i].Id() == GameObject::InvalidID)
			{
				return i;
			}
		}
		BLT_ASSERT(false, "Unable to find valid Id for Camera");
		return GameObject::InvalidID;
	}

}