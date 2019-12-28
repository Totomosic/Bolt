#include "bltpch.h"
#include "Scene.h"

namespace Bolt
{

	Scene::Scene()
		: m_Layers()
	{
	}

	Layer& Scene::AddLayer()
	{
		size_t index = m_Layers.size();
		m_Layers.push_back(std::make_unique<Layer>());
		return *m_Layers.at(index);
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

	void Scene::Load()
	{
	}

	void Scene::Unload()
	{
	}

	void Scene::Update(TimeDelta delta)
	{
		for (const std::unique_ptr<Layer>& layer : m_Layers)
		{
			layer->Update(delta);
		}
	}

	void Scene::Render(TimeDelta delta)
	{
		for (const std::unique_ptr<Layer>& layer : m_Layers)
		{
			layer->Render(delta);
		}
	}

}