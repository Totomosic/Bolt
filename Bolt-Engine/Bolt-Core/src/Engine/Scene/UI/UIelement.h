#pragma once
#include "../GameObject.h"
#include "UIEventHandler.h"

#include "Graphics/Resources/Meshes/Mesh.h"
#include "Graphics/Resources/Meshes/Factories/TextFactory.h"

namespace Bolt
{

	class UIManager;
	class UISurface;
	class UIText;

	class BLT_API UIElement
	{
	protected:
		UIManager* m_Manager;
		GameObject* m_GameObject;
		UIElement* m_Parent;
		std::vector<std::unique_ptr<UIElement>> m_Children;		
		UIEventHandler m_Events;
		bool m_IsFocused;

	public:
		UIElement(UIManager* manager, UIElement* parent);
		UIElement(UIElement&& other) = delete;
		UIElement& operator=(UIElement&& other) = delete;
		~UIElement();

		UIElement& GetParent() const;
		bool HasParent() const;
		const std::vector<std::unique_ptr<UIElement>>& GetChildren() const;

		const Transform& GetTransform() const;
		bool HasMesh() const;
		const Mesh& GetMesh() const;
		Mesh& GetMesh();
		const UIEventHandler& Events() const;
		UIEventHandler& Events();
		bool HasFocus() const;

		GameObject* GetGameObject() const;
		UIManager* GetManager() const;

		void Focus();
		void Blur();
		void Clear();

		virtual bool ContainsPoint(const Vector2f& screenPoint) const;

		UIElement& CreateElement();
		UISurface& CreateSurface(float width, float height, std::unique_ptr<Material>&& material, Transform&& transform = Transform());
		UISurface& CreateSurface(float width, float height, const Color& color, Transform&& transform = Transform());
		UIText& CreateText(const blt::string& text, const ResourcePtr<Font>& font, const Color& color = Color::Black, Transform&& transform = Transform(), AlignH horizontal = AlignH::Center, AlignV vertical = AlignV::Center);
		UIText& CreateText(const blt::string& text, const Color& color = Color::Black, Transform&& transform = Transform(), AlignH horizontal = AlignH::Center, AlignV vertical = AlignV::Center);

	protected:
		void SetGameObject(GameObject* object);
		UIElement& AddChildElement(std::unique_ptr<UIElement>&& element);

	};

}