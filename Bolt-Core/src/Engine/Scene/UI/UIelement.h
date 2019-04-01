#pragma once
#include "..\GameObject.h"
#include "..\Components\UI\UIEventHandler.h"
#include "..\..\..\Graphics\Resources\__Resources__.h"

namespace Bolt
{

	class UIroot;
	class UIsurface;
	class Text;

	class BLT_API UIelement
	{
	protected:
		UIroot* m_Root;
		UIelement* m_Parent;
		std::vector<std::unique_ptr<UIelement>> m_Children;
		GameObject* m_Object;

	public:
		UIelement();
		UIelement(GameObject* object);
		virtual ~UIelement();

		GameObject* Object() const;
		UIelement& Parent() const;
		UIEventHandler& EventHandler() const;
		UIelement& GetElement(id_t index) const;
		UIelement& AddElement(std::unique_ptr<UIelement>&& element);
		void RemoveElement(UIelement* element);
		void ReleaseGameObjects();
		void Clear();

		template<typename T, typename ...Args>
		T& AddElement(Args&&... args)
		{
			return *(T*)&AddElement(std::make_unique<T>(std::move(args)...));
		}

		UIsurface& Rectangle(float width, float height, const Color& color = Color::White, Transform&& transform = Transform());
		UIsurface& Rectangle(float width, float height, std::unique_ptr<Material>&& material, Transform&& transform = Transform());
		UIsurface& Image(float width, float height, const ResourcePtr<const Texture2D>& texture, Transform&& transform = Transform());
		Bolt::Text& Text(const blt::string& text, const ResourcePtr<const Font>& font, const Color& color = Color::White, Transform&& transform = Transform(), AlignH horizontal = AlignH::Center, AlignV vertical = AlignV::Center);
		Bolt::Text& Text(const blt::string& text, const Color& color = Color::White, Transform&& transform = Transform(), AlignH horizontal = AlignH::Center, AlignV vertical = AlignV::Center);

		friend class UIroot;
		friend struct Layer;

	protected:
		virtual void SetUIroot(UIroot* root);
		void SetParent(UIelement* parent);

	};

}