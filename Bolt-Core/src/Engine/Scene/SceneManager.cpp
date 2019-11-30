#include "bltpch.h"
#include "SceneManager.h"
#include "Engine/Engine.h"

#include "Core/Profiling/Profiling.h"

namespace Bolt
{

	SceneManager& SceneManager::Get()
	{
		return Engine::Instance().CurrentContext().GetRenderContext().GetSceneManager();
	}

	SceneManager::SceneManager()
		: m_Scenes(), m_SceneMap(), m_CurrentScene(nullptr)
	{
		
	}

	Scene& SceneManager::CurrentScene()
	{
		return *m_CurrentScene;
	}

	Scene& SceneManager::GetSceneById(id_t id)
	{
		return *m_Scenes[id].get();
	}

	Scene& SceneManager::GetSceneByName(const blt::string& name)
	{
		BLT_PROFILE_FUNCTION();
		auto it = m_SceneMap.find(name);
		if (it != m_SceneMap.end())
		{
			return *(*it).second;
		}
		BLT_ASSERT(false, "No scene with name " + name + " was found");
		return *(Scene*)nullptr;
	}

	Scene& SceneManager::CreateScene(int layerCount, const blt::string& name)
	{
		BLT_PROFILE_FUNCTION();
		id_t index = m_Scenes.size();
		std::unique_ptr<Scene> s = std::make_unique<Scene>(layerCount);
		Scene* ptr = s.get();
		m_Scenes.push_back(std::move(s));
		ptr->m_Id = index;
		if (!name.empty())
		{
			m_SceneMap[name] = ptr;
		}
		if (m_CurrentScene == nullptr)
		{
			m_CurrentScene = ptr;
			m_CurrentScene->SetIsActive(true);
		}
		return *ptr;
	}

	void SceneManager::DisableCurrentScene()
	{
		BLT_CORE_WARN("No Current Scene");
		SetCurrentScene(*(Scene*)nullptr);
	}

	void SceneManager::SetCurrentScene(Scene& scene)
	{
		BLT_PROFILE_FUNCTION();
		if (m_CurrentScene != nullptr)
		{
			SceneUnloadedEvent e;
			e.UnloadedScene = m_CurrentScene;
			m_CurrentScene->SetIsActive(false);
			m_CurrentScene->OnUnload.Emit(std::move(e));
		}
		m_CurrentScene = &scene;
		if (m_CurrentScene != nullptr)
		{
			SceneLoadedEvent e;
			e.LoadedScene = m_CurrentScene;
			e.LoadData = nullptr;
			m_CurrentScene->SetIsActive(true);
			m_CurrentScene->OnLoad.Emit(std::move(e));
		}
	}

	void SceneManager::SetCurrentSceneById(id_t id)
	{
		SetCurrentScene(GetSceneById(id));
	}

	void SceneManager::SetCurrentSceneByName(const blt::string& name)
	{
		SetCurrentScene(GetSceneByName(name));
	}

}