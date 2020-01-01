#pragma once
#include "../../EntityManager.h"
#include "../../EntityFactory.h"
#include "../../Components/UIEvents.h"
#include "UIFactory.h"

namespace Bolt
{

	class UIManager;

	class BLT_API UIElement : public UIFactory
	{
	private:
		UIManager* m_Manager;
		EntityHandle m_Entity;
		mutable bool m_IsFocused;

		UIElement* m_Parent;
		std::vector<std::unique_ptr<UIElement>> m_Children;
		UIElement* m_CompoundElement;

	public:
		UIElement(UIManager* manager, UIElement* parent);
		UIElement(UIManager* manager, UIElement* parent, const EntityHandle& entity);
		UIElement(const UIElement& other) = delete;
		UIElement& operator=(const UIElement& other) = delete;
		UIElement(UIElement&& other) = delete;
		UIElement& operator=(UIElement&& other) = delete;
		virtual ~UIElement();

		bool HasParent() const;
		UIElement& GetParent() const;
		const std::vector<std::unique_ptr<UIElement>>& GetChildren() const;
		EntityHandle GetEntity() const;
		const UIEvents& Events() const;
		UIEvents& Events();

		const Transform& GetTransform() const;
		Transform& GetTransform();
		bool HasMesh() const;
		const Mesh& GetMesh() const;
		Mesh& GetMesh();
		bool HasFocus() const;

		UIElement& AddChild(std::unique_ptr<UIElement>&& element);
		void RemoveChild(UIElement& element);
		void ClearChildren();

		bool HasCompoundElement() const;
		UIElement& GetCompoundElement() const;
		void SetCompoundElement(UIElement* element);

		// Removes this element
		void Remove();

		void Focus();
		void Blur();

		virtual bool ContainsPoint(const Vector2f& point) const;

	protected:
		const EntityFactory& GetEntityFactory() const;
		EntityHandle SetEntity(EntityHandle entity);
		bool ValidateEntity(EntityHandle entity) const;

	private:
		void SetParent(UIElement* element);
		void SetupEntity(EntityHandle entity) const;
	};

}