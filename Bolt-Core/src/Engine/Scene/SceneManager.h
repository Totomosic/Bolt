#pragma once
#include "Scene.h"

namespace Bolt
{

	class BLT_API SceneManager
	{
	private:
		std::vector<Scene> m_Scenes;
		Scene* m_CurrentScene;

	public:
		static SceneManager& Get();

	public:
		SceneManager();

		Scene& AddScene();
		void RemoveScene(Scene& scene);

		bool HasCurrentScene() const;
		Scene& GetCurrentScene() const;
		void SetCurrentScene(Scene& scene);
		void SetCurrentScenePtr(Scene* scene);
	};

}