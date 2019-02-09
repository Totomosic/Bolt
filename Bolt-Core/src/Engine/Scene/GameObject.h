#pragma once
#include "ObjectPrefab.h"

namespace Bolt
{

	struct Layer;

	// Class that represents any object in the game
	class BLT_API GameObject : public ObjectPrefab
	{
	public:
		static constexpr id_t InvalidID = (id_t)-1;

		struct BLT_API TempComponent
		{
		public:
			Component* component;
			float TimeToDelete;
		};

	protected:
		id_t m_Id;
		GameObject* m_Parent;
		std::vector<GameObject*> m_Children;
		std::vector<blt::string> m_Tags;
		Layer* m_Layer;
		std::vector<TempComponent> m_TemporaryComponents;

	protected:
		// Do not call directly, Use GameObject::Instantiate()
		GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;

	public:
		GameObject(GameObject&& other);
		GameObject& operator=(GameObject&& other);
		virtual ~GameObject();

	public:
		GameObject* Parent() const;
		Layer* GetLayer() const;
		id_t Id() const;
		const std::vector<blt::string>& Tags() const;

		void MakeChildOf(GameObject* parent);
		void MakeStandalone();
		void AddTag(const blt::string& tag);
		void RemoveTag(const blt::string& tag);
		void RemoveAllTags();

		void Update();
		void LateUpdate();

		ObjectPrefab GetPrefab() const;

		void Transfer(XMLserializer& backend, bool isWriting) override;

		friend void Destroy(Component* component, float timeToDelete = 0.0f);

		friend struct ObjectCollection;
		friend struct Layer;
		friend struct Scene;
		friend class Graphics;

	private:
		void SetLayer(Layer* layer);
		void AddTagPrivate(const blt::string& tag);
		void MarkComponentForDelete(Component* component, float timeToDelete);
		void OnDestroy();
		void SetId(id_t id);

	public:
		static GameObject* Instantiate(Layer* layer, Transform transform = Transform());
		static GameObject* Instantiate(Layer* layer, float x, float y, float z, const Quaternion& orientation = Quaternion::Identity(), const Vector3f& scale = Vector3f(1, 1, 1));
		static GameObject* Instantiate(Layer* layer, const ObjectPrefab* prefab);
		static GameObject* Instantiate(Layer* layer, const ObjectPrefab* prefab, Transform transform);
		static GameObject* Instantiate(Layer* layer, const ObjectPrefab* prefab, GameObject* parent);
		static GameObject* Instantiate(Layer* layer, const ObjectPrefab* prefab, GameObject* parent, Transform transform);

	};

}