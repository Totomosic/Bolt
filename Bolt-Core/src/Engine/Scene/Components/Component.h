#pragma once
#include "Bolt-Graphics.h"

namespace Bolt
{

	class ObjectPrefab;
	class GameObject;
	class Camera;
	struct Layer;

	class BLT_API Component
	{
	protected:
		ObjectPrefab* m_GameObject;
		id_t m_Id;

	public:
		Component();
		Component(id_t m_Id);
		virtual ~Component();

		ObjectPrefab* objectPrefab() const;
		GameObject* gameObject() const;
		Camera* camera() const;
		Layer* layer() const;

		virtual void Start();
		virtual void Update();
		virtual void LateUpdate();
		virtual void End();

		virtual std::unique_ptr<Component> Clone() const = 0;
		virtual void Transfer(XMLserializer& backend, bool isWriting);

		friend class GameObject;
		friend class ComponentManager;

	private:
		void SetGameObject(ObjectPrefab* object);

	};

}