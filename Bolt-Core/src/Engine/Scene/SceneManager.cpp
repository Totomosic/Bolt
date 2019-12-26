#include "bltpch.h"
#include "SceneManager.h"

#include "Engine/Engine.h"

namespace Bolt
{

	SceneManager& SceneManager::Get()
	{
		return Engine::Instance().CurrentContext().GetRenderContext().GetSceneManager();
	}

	SceneManager::SceneManager()
		: m_Scenes(), m_CurrentScene(nullptr)
	{
	}

	Scene& SceneManager::AddScene()
	{
		size_t index = m_Scenes.size();
		m_Scenes.push_back(Scene());
		Scene& scene = m_Scenes[index];
		if (!HasCurrentScene())
		{
			SetCurrentScene(scene);
		}
		return scene;
	}

	void SceneManager::RemoveScene(Scene& scene)
	{
		auto it = std::find_if(m_Scenes.begin(), m_Scenes.end(), [&scene](const Scene& s)
			{
				return &scene == &s;
			});
		if (it != m_Scenes.end())
		{
			if (&*it == &GetCurrentScene())
			{
				SetCurrentScenePtr(nullptr);
			}
			m_Scenes.erase(it);
		}
	}

	bool SceneManager::HasCurrentScene() const
	{
		return m_CurrentScene != nullptr;
	}

	Scene& SceneManager::GetCurrentScene() const
	{
		return *m_CurrentScene;
	}

	void SceneManager::SetCurrentScene(Scene& scene)
	{
		SetCurrentScenePtr(&scene);
	}

	void SceneManager::SetCurrentScenePtr(Scene* scene)
	{
		if (m_CurrentScene != nullptr)
		{
			m_CurrentScene->Unload();
		}
		m_CurrentScene = scene;
		if (m_CurrentScene != nullptr)
		{
			m_CurrentScene->Load();
		}
	}

}