#pragma once
#include "../../EntityManager.h"
#include "../../EntityFactory.h"
#include "../../Components/UIEvents.h"
#include "UIFactory.h"

namespace Bolt
{

	class UIManager;

	class BLT_API UIElement
	{
	private:
		UIManager* m_Manager;
		EntityHandle m_Entity;
		mutable bool m_IsFocused;

		UIElement* m_Parent;
		std::vector<std::unique_ptr<UIElement>> m_Children;

	public:
		UIElement(UIManager* manager);
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
		UIFactory GetFactory();

		const Transform& GetTransform() const;
		Transform& GetTransform();
		bool HasMesh() const;
		const Mesh& GetMesh() const;
		Mesh& GetMesh();
		bool HasFocus() const;

		UIElement& AddChild(std::unique_ptr<UIElement>&& element);
		void RemoveChild(UIElement& element);
		void ClearChildren();

		// Removes this element
		void Remove();

		void Focus() const;
		void Blur() const;

		virtual bool ContainsPoint(const Vector2f& point) const;

	protected:
		const EntityFactory& GetFactory() const;
		EntityHandle SetEntity(EntityHandle entity) const;
		bool ValidateEntity(EntityHandle entity) const;
	};

}