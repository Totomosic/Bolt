#include "bltpch.h"
#include "Scene.h"

namespace Bolt
{

	Scene::Scene()
		: m_Layers(), m_IsLoaded(false), m_IsActive(false),
		m_SharedEntities(), m_Systems(m_SharedEntities), m_OnLoad(m_SharedEntities.Bus().GetEmitter<SceneLoadEvent>()), m_OnUnload(m_SharedEntities.Bus().GetEmitter<SceneUnloadEvent>())
	{
	}

	const EventEmitter<SceneLoadEvent>& Scene::OnLoad() const
	{
		return m_OnLoad;
	}

	const EventEmitter<SceneUnloadEvent>& Scene::OnUnload() const
	{
		return m_OnUnload;
	}

	EventEmitter<SceneLoadEvent>& Scene::OnLoad()
	{
		return m_OnLoad;
	}

	EventEmitter<SceneUnloadEvent>& Scene::OnUnload()
	{
		return m_OnUnload;
	}

	const EntityManager& Scene::SharedEntities() const
	{
		return m_SharedEntities;
	}

	EntityManager& Scene::SharedEntities()
	{
		return m_SharedEntities;
	}

	const SystemRegistry& Scene::Systems() const
	{
		return m_Systems;
	}

	SystemRegistry& Scene::Systems()
	{
		return m_Systems;
	}

	EntityFactory Scene::GetFactory()
	{
		return EntityFactory(m_SharedEntities);
	}

	Layer& Scene::AddLayer()
	{
		size_t index = m_Layers.size();
		m_Layers.push_back(std::make_unique<Layer>());
		Layer& layer = *m_Layers.at(index);
		if (!IsActive())
		{
			layer.Disable();
		}
		return layer;
	}

	void Scene::RemoveLayer(Layer& layer)
	{
		auto it = std::find_if(m_Layers.begin(), m_Layers.end(), [&layer](const std::unique_ptr<Layer>& l)
			{
				return &layer == l.get();
			});
		if (it != m_Layers.end())
		{
			m_Layers.erase(it);
		}
	}

	bool Scene::IsLoaded() const
	{
		return m_IsLoaded;
	}

	bool Scene::IsActive() const
	{
		return m_IsActive && IsLoaded();
	}

	void Scene::SetIsActive(bool isActive)
	{
		if (m_IsActive != isActive)
		{
			m_IsActive = isActive;
		}
	}

	void Scene::Clear()
	{
		m_Layers.clear();
		m_SharedEntities.Clear();
	}

	void Scene::Load(const std::any& data)
	{
		if (!IsLoaded())
		{
			OnLoad().Emit({ *this, data });
			m_IsLoaded = true;
			for (auto& layer : m_Layers)
			{
				layer->Enable();
			}
		}
	}

	void Scene::Unload(const std::any& data)
	{
		if (IsLoaded())
		{
			OnUnload().Emit({ *this, data });
			m_IsLoaded = false;
			for (auto& layer : m_Layers)
			{
				layer->Disable();
			}
		}
	}

	void Scene::Update(TimeDelta delta)
	{
		if (IsActive())
		{
			Systems().UpdateAll(delta);
			for (const std::unique_ptr<Layer>& layer : m_Layers)
			{
				layer->Update(delta);
			}
		}
	}

	void Scene::Render(TimeDelta delta)
	{
		if (IsActive())
		{
			for (const std::unique_ptr<Layer>& layer : m_Layers)
			{
				layer->Render(delta);
			}
		}
	}

}