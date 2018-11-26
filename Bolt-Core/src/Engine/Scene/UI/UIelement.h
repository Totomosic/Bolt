#pragma once
#include "..\GameObject.h"
#include "..\Components\__Components__.h"

namespace Bolt
{

	struct Layer;
	class Text;
	class UIsurface;

	class BLT_API UIelement
	{
	protected:
		id_t m_Id;
		UIelement* m_ParentElement;
		GameObject* m_GameObject;

		std::vector<std::unique_ptr<UIelement>> m_Elements;

	public:
		UIelement();
		UIelement(GameObject* object);
		virtual ~UIelement();

		id_t Id() const;
		GameObject* Object() const;
		UIEventHandler& EventHandler() const;
		UIelement* Parent() const;
		UIelement* GetElementById(id_t id) const;

		template<typename T>
		T* GetElementById(id_t id) const
		{
			return (T*)GetElementById(id);
		}

		int ElementCount() const;

		void SetParent(UIelement* parent);
		UIelement* AddElement(std::unique_ptr<UIelement>&& element);
		void Clear();
		void ReleaseAllGameObjects();

		template<typename T, typename ...Args>
		UIelement* AddElement(Args&& ...args)
		{
			return AddElement(std::make_unique<T>(std::move(args)...));
		}

		// Helper Functions
		Bolt::Text* Text(const blt::string& text, const ResourcePtr<const Font>& font, const Color& color = Color::White, Transform&& transform = Transform(), AlignH horizontal = AlignH::Center, AlignV vertical = AlignV::Center);
		Bolt::Text* Text(const blt::string& text, const Color& color = Color::White, Transform&& transform = Transform(), AlignH horizontal = AlignH::Center, AlignV vertical = AlignV::Center);
		UIsurface* Rectangle(float width, float height, const Color& color = Color::White, Transform&& transform = Transform());
		UIsurface* Rectangle(float width, float height, const Material& material, Transform&& transform = Transform());
		UIsurface* Image(float width, float height, const ResourcePtr<const Texture2D>& texture, Transform&& transform = Transform());

	protected:
		void SetId(id_t id);
		void SetGameObject(GameObject* object);
		virtual void CreateGameObject();

	};

}