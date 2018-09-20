#pragma once
#include "VertexFactory.h"
#include "..\..\Textures\Fonts\Font.h"
#include "..\..\ResourcePtr.h"

namespace Bolt
{

	enum class AlignH
	{
		Left,
		Center,
		Right
	};

	enum class AlignV
	{
		Bottom,
		Center,
		Top
	};

	class BLT_API TextFactory : public VertexFactory
	{
	public:
		ResourcePtr<const Font> TextFont;
		blt::string Text;
		Bolt::Color Color;
		AlignH HorizontalAlign;
		AlignV VerticalAlign;

	public:
		TextFactory(const blt::string& text, const ResourcePtr<const Font>& font, const Bolt::Color& color = Color::White, AlignH horizontalAlignment = AlignH::Center, AlignV verticalAlignment = AlignV::Center);

		ModelData GenerateVertices() const override;

	};

}