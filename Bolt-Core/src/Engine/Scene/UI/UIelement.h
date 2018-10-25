#pragma once
#include "..\GameObject.h"
#include "..\Components\__Components__.h"

namespace Bolt
{

	struct Layer;

	class BLT_API UIelement
	{
	protected:
		UIelement* m_ParentElement;
		GameObject* m_GameObject;

		std::vector<std::unique_ptr<UIelement>> m_Elements;

	public:
		UIelement();
		UIelement(GameObject* object);
		virtual ~UIelement();

		GameObject* Object() const;
		UIelement* Parent() const;
		UIelement* GetElementById(id_t id) const;

		template<typename T>
		T* GetElementById(id_t id) const
		{
			return (T*)GetElementById(id);
		}

		int ElementCount() const;

		void SetParent(UIelement* parent);
		id_t AddElement(std::unique_ptr<UIelement>&& element);
		void Clear();

	protected:
		void SetGameObject(GameObject* object);
		virtual void CreateGameObject();

	};

}