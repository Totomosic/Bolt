#pragma once
#include "VertexFactory.h"
#include "..\..\Textures\Fonts\Font.h"

namespace Bolt
{

	enum class TextAlignmentH
	{
		Left,
		Center,
		Right
	};

	class BLT_API TextFactory : public VertexFactory
	{
	public:
		const Font* TextFont;
		blt::string Text;
		Bolt::Color Color;
		TextAlignmentH HorizontalAlign;

	public:
		TextFactory(const blt::string& text, const Font* font, const Bolt::Color& color = Color::White, TextAlignmentH horizontalAlignment = TextAlignmentH::Center);

		ModelData GenerateVertices() const override;

	};

}