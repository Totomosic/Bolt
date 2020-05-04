#pragma once
#include "../../Components/Transform.h"
#include "Graphics/Assets/Meshes/Mesh.h"
#include "Graphics/Assets/Meshes/Factories/TextFactory.h"

namespace Bolt
{

	class UIManager;

	class UIElement;
	class UIRectangle;
	class UIText;
	class UITextInput;

	class BLT_API UIFactory
	{
	private:
		UIManager* m_Manager;
		UIElement& m_ParentElement;

	public:
		UIFactory(UIManager* manager, UIElement& parentElement);

		UIElement& CreateElement(Transform transform = Transform());
		UIRectangle& CreateRectangle(float width, float height, std::unique_ptr<Material>&& material, Transform transform = Transform());
		UIRectangle& CreateRectangle(float width, float height, const Color& color, Transform transform = Transform());
		UIRectangle& CreateImage(float width, float height, const AssetHandle<Texture2D>& image, Transform transform = Transform());

		UIText& CreateText(const std::string& text, const AssetHandle<Font>& font, const Color& color, 
			Transform transform = Transform(), AlignH horizontal = AlignH::Center, AlignV vertical = AlignV::Center);
		UIText& CreateText(const std::string& text, const Color& color,
			Transform transform = Transform(), AlignH horizontal = AlignH::Center, AlignV vertical = AlignV::Center);

		UITextInput& CreateTextInput(float width, float height, const AssetHandle<Font>& font, const Color& fontColor, std::unique_ptr<Material>&& backgroundMaterial, Transform transform = Transform());
		UITextInput& CreateTextInput(float width, float height, const Color& fontColor, std::unique_ptr<Material>&& backgroundMaterial, Transform transform = Transform());
		UITextInput& CreateTextInput(float width, float height, const AssetHandle<Font>& font, const Color& fontColor, const Color& backgroundColor, Transform transform = Transform());
		UITextInput& CreateTextInput(float width, float height, const Color& fontColor, const Color& backgroundColor, Transform transform = Transform());

	};

}