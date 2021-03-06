#include "bltpch.h"
#include "SceneManager.h"

#include "Engine/Engine.h"

namespace Bolt
{

	SceneManager& SceneManager::Get()
	{
		return Engine::Instance().CurrentContext().GetSceneManager();
	}

	SceneManager::SceneManager()
		: m_Scenes(), m_CurrentScene(nullptr)
	{
	}

	Scene& SceneManager::AddScene()
	{
		size_t index = m_Scenes.size();
		m_Scenes.push_back(std::make_unique<Scene>());
		Scene& scene = *m_Scenes[index];
		if (!HasCurrentScene())
		{
			SetCurrentScene(scene);
		}
		return scene;
	}

	void SceneManager::RemoveScene(Scene& scene)
	{
		auto it = std::find_if(m_Scenes.begin(), m_Scenes.end(), [&scene](const std::unique_ptr<Scene>& s)
			{
				return &scene == s.get();
			});
		if (it != m_Scenes.end())
		{
			if ((*it).get() == &GetCurrentScene())
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

	void SceneManager::SetCurrentScene(Scene& scene, const std::any& loadData, const std::any& unloadData)
	{
		SetCurrentScenePtr(&scene, loadData, unloadData);
	}

	void SceneManager::SetCurrentScenePtr(Scene* scene, const std::any& loadData, const std::any& unloadData)
	{
		if (m_CurrentScene != nullptr)
		{
			m_CurrentScene->SetIsActive(false);
			m_CurrentScene->Unload(unloadData);
		}
		m_CurrentScene = scene;
		if (m_CurrentScene != nullptr)
		{
			m_CurrentScene->Load(loadData);
			m_CurrentScene->SetIsActive(true);
		}
	}

}