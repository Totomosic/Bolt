#pragma once
#include "ObjectCollection.h"
#include "Query/SGQuery.h"
#include "LayerEvents.h"
#include "UI/UIManager.h"

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
		bool m_Enabled;
		Camera* m_ActiveCamera;
		
		UIManager m_UIManager;
		std::vector<TempGameObject> m_TemporaryObjects;

	public:
		Layer();
		Layer(const Layer& other) = delete;
		Layer& operator=(const Layer& other) = delete;
		Layer(Layer&& other) = default;
		Layer& operator=(Layer&& other) = default;
		~Layer() = default;

		const ObjectCollection& GameObjects() const;
		ObjectCollection& GameObjects();
		Camera* ActiveCamera() const;
		id_t Id() const;
		const UIManager& UI() const;
		UIManager& UI();

		bool IsEnabled() const;
		void Enable();
		void Disable();
		void SetEnabled(bool isEnabled);
		void SetActiveCamera(Camera* camera);

		GameObject* AddGameObject(GameObject&& object);
		void RemoveGameObject(GameObject* object);
		void RemoveGameObject(id_t id);
		void Clear();

		void Update();
		void UpdateTemporaryObjects();

		void Transfer(XMLserializer& backend, bool isWriting);

		// Deletes given object after timeToDelete seconds. Will not be deleted instantly.
		friend void BLT_API Destroy(GameObject* object, float timeToDelete = 0.0f);

		friend struct Scene;
		friend class Graphics;
		friend class Component;

	private:
		GameObject* AddTemporaryGameObject(GameObject&& object);
		void MarkGameObjectForDelete(GameObject* object, float timeToDelete = 0.0f);
		void Create(id_t id, int maxGameObjects);

	};

}