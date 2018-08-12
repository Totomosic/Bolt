#pragma once
#include "Bolt-Graphics.h"
#include "SceneGraph\__SceneGraph__.h"
#include "ObjectCollection.h"

namespace Bolt
{

	struct BLT_API Layer
	{
	private:
		struct BLT_API TempGameObject
		{
		public:
			GameObject* Object;
			float TimeToDelete;
		};

	private:
		id_t m_Id;
		ObjectCollection m_GameObjects;
		std::unique_ptr<SceneGraph> m_SceneGraph;
		bool m_Enabled;
		Camera* m_ActiveCamera;
		
		std::vector<TempGameObject> m_TemporaryObjects;

	public:
		Layer();
		Layer(const Layer& other) = delete;
		Layer& operator=(const Layer& other) = delete;
		Layer(Layer&& other) = default;
		Layer& operator=(Layer&& other) = default;
		~Layer() = default;

		const ObjectCollection& GameObjects() const;
		const SceneGraph& Graph() const;
		Camera* ActiveCamera() const;
		id_t Id() const;

		bool IsEnabled() const;
		void Enable();
		void Disable();
		void SetActiveCamera(Camera* camera);

		GameObject* AddGameObject(GameObject&& object);
		void RemoveGameObject(GameObject* object);
		void RemoveGameObject(id_t id);
		void Clear();

		void Update();
		void UpdateTemporaryObjects();
		void MarkGameObjectForDelete(GameObject* object, float timeToDelete = 0.0f);

		friend struct Scene;
		friend class Graphics;
		friend class Component;

	private:
		GameObject* AddTemporaryGameObject(GameObject&& object);

	};

	void Destroy(GameObject* object, float timeToDelete);

}