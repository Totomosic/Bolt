#include "SceneManager.h"

namespace Bolt
{

	std::vector<std::unique_ptr<Scene>> SceneManager::s_Scenes = std::vector<std::unique_ptr<Scene>>();
	Scene* SceneManager::s_CurrentScene = nullptr;
	Window* SceneManager::s_Window = nullptr;

	Scene& SceneManager::CurrentScene()
	{
		return *s_CurrentScene;
	}

	Scene* SceneManager::CreateScene()
	{
		int index = s_Scenes.size();
		s_Scenes.push_back(std::make_unique<Scene>());
		Scene* ptr = s_Scenes[index].get();
		if (s_CurrentScene == nullptr)
		{
			s_CurrentScene = ptr;
		}
		return ptr;
	}

	void SceneManager::SetCurrentScene(Scene* scene)
	{
		s_CurrentScene = scene;
	}

	void SceneManager::Terminate()
	{
		s_Scenes.clear();
		s_CurrentScene = nullptr;
		s_Window = nullptr;
	}

}