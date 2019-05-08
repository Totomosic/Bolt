#include "Types.h"

#include "SceneManager.h"
#include "Engine/Engine.h"

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
		}
		return *ptr;
	}

	void SceneManager::SetCurrentScene(Scene& scene)
	{
		if (m_CurrentScene != nullptr)
		{
			SceneUnloadedEvent e;
			e.UnloadedScene = m_CurrentScene;
			m_CurrentScene->OnUnload.Post(std::move(e));
		}
		m_CurrentScene = &scene;
		if (m_CurrentScene != nullptr)
		{
			SceneLoadedEvent e;
			e.LoadedScene = m_CurrentScene;
			e.LoadData = nullptr;
			m_CurrentScene->OnLoad.Post(std::move(e));
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