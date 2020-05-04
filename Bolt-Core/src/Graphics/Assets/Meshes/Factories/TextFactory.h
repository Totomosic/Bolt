#pragma once
#include "VertexFactory.h"
#include "Graphics/Assets/Textures/Fonts/Font.h"
#include "Graphics/Assets/AssetHandle.h"

namespace Bolt
{

	BLT_API enum class AlignH
	{
		Left,
		Center,
		Right
	};

	BLT_API enum class AlignV
	{
		Bottom,
		Center,
		Top
	};

	class BLT_API TextFactory : public VertexFactory
	{
	public:
		AssetHandle<Font> TextFont;
		std::string Text;
		Bolt::Color Color;
		AlignH HorizontalAlign;
		AlignV VerticalAlign;

	public:
		TextFactory(const std::string& text, const AssetHandle<Font>& font, const Bolt::Color& color = Color::White, AlignH horizontalAlignment = AlignH::Center, AlignV verticalAlignment = AlignV::Center);

		MeshData GenerateVertices() const override;

	};

}