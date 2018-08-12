#pragma once
#include "Bolt-Graphics.h"

namespace Bolt
{

	class GameObject;
	struct Layer;

	class BLT_API Component
	{
	protected:
		GameObject* m_GameObject;
		id_t m_Id;

	public:
		Component();
		Component(id_t m_Id);
		virtual ~Component();

		GameObject* gameObject() const;
		Layer* layer() const;

		virtual void Start();
		virtual void Update();
		virtual void LateUpdate();
		virtual void End();

		virtual std::unique_ptr<Component> Clone() const = 0;

		friend class GameObject;
		friend class ComponentManager;

	private:
		void SetGameObject(GameObject* object);

	};

}