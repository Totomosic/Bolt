#include "Types.h"

#include "SceneManager.h"

namespace Bolt
{

	std::vector<std::unique_ptr<Scene>> SceneManager::s_Scenes = std::vector<std::unique_ptr<Scene>>();
	std::unordered_map<blt::string, Scene*> SceneManager::s_SceneMap = std::unordered_map<blt::string, Scene*>();
	Scene* SceneManager::s_CurrentScene = nullptr;
	Window* SceneManager::s_Window = nullptr;

	Scene& SceneManager::CurrentScene()
	{
		return *s_CurrentScene;
	}

	Scene& SceneManager::GetSceneById(id_t id)
	{
		return *s_Scenes[id].get();
	}

	Scene& SceneManager::GetSceneByName(const blt::string& name)
	{
		auto it = s_SceneMap.find(name);
		if (it != s_SceneMap.end())
		{
			return *(*it).second;
		}
		BLT_ASSERT(false, "No scene with name " + name + " was found");
		return *(Scene*)nullptr;
	}

	Scene& SceneManager::CreateScene(const blt::string& name)
	{
		id_t index = s_Scenes.size();
		std::unique_ptr<Scene> s = std::make_unique<Scene>();
		Scene* ptr = s.get();
		s_Scenes.push_back(std::move(s));
		ptr->m_Id = index;
		if (!name.empty())
		{
			s_SceneMap[name] = ptr;
		}
		if (s_CurrentScene == nullptr)
		{
			s_CurrentScene = ptr;
		}
		return *ptr;
	}

	void SceneManager::SetCurrentScene(Scene& scene)
	{
		if (s_CurrentScene != nullptr)
		{
			std::unique_ptr<SceneUnloadedEvent> e = std::make_unique<SceneUnloadedEvent>();
			e->UnloadedScene = s_CurrentScene;
			s_CurrentScene->OnUnload.Post(std::move(e));
		}
		s_CurrentScene = &scene;
		if (s_CurrentScene != nullptr)
		{
			std::unique_ptr<SceneLoadedEvent> e = std::make_unique<SceneLoadedEvent>();
			e->LoadedScene = s_CurrentScene;
			s_CurrentScene->OnLoad.Post(std::move(e));
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

	void SceneManager::Terminate()
	{
		s_Scenes.clear();
		s_CurrentScene = nullptr;
		s_Window = nullptr;
	}

}