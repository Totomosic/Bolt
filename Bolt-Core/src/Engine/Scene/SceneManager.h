#pragma once
#include "Scene.h"

namespace Bolt
{

	class BLT_API SceneManager
	{
	private:
		std::vector<std::unique_ptr<Scene>> m_Scenes;
		Scene* m_CurrentScene;

	public:
		static SceneManager& Get();

	public:
		SceneManager();

		Scene& AddScene();
		void RemoveScene(Scene& scene);

		bool HasCurrentScene() const;
		Scene& GetCurrentScene() const;
		void SetCurrentScene(Scene& scene, const std::any& loadData = {}, const std::any& unloadData = {});
		void SetCurrentScenePtr(Scene* scene, const std::any& loadData = {}, const std::any& unloadData = {});
	};

}