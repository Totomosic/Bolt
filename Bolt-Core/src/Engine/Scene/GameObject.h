#pragma once
#include "Bolt-Graphics.h"
#include "Transform.h"
#include "Components\ComponentManager.h"

namespace Bolt
{

	struct Layer;

	// Class that represents any object in the game
	class BLT_API GameObject
	{
	public:
		static constexpr id_t InvalidID = (id_t)-1;

	protected:
		id_t m_Id;
		Transform m_Transform;
		ComponentManager m_Components;
		GameObject* m_Parent;
		std::vector<GameObject*> m_Children;
		Layer* m_Layer;

	public:
		// Do not call directly, Use GameObject::Instantiate()
		GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject(GameObject&& other);
		GameObject& operator=(GameObject&& other);
		virtual ~GameObject();

	public:
		const Transform& transform() const;
		Transform& transform();
		const ComponentManager& Components() const;
		ComponentManager& Components();
		GameObject* Parent() const;
		Layer* GetLayer() const;
		id_t ID() const;

		void MakeChildOf(GameObject* parent);
		void MakeStandalone();

		void SetID(id_t id);

		void Update();
		void LateUpdate();

		friend struct ObjectCollection;
		friend struct Layer;

	private:
		void SetLayer(Layer* layer);

	public:
		static GameObject* Instantiate(Layer* layer, Transform transform = Transform());
		static GameObject* Instantiate(Layer* layer, float x, float y, float z, const Quaternion& orientation = Quaternion::Identity(), const Vector3f& scale = Vector3f(1, 1, 1));
		static GameObject* Instantiate(Layer* layer, const GameObject* prefab);
		static GameObject* Instantiate(Layer* layer, const GameObject* prefab, Transform transform);
		static GameObject* Instantiate(Layer* layer, const GameObject* prefab, GameObject* parent);
		static GameObject* Instantiate(Layer* layer, const GameObject* prefab, GameObject* parent, Transform transform);

	};

}