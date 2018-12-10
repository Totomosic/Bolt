#include "Types.h"
#include "TextFactory.h"

namespace Bolt
{

	TextFactory::TextFactory(const blt::string& text, const ResourcePtr<const Font>& font, const Bolt::Color& color, AlignH horizontalAlignment, AlignV verticalAlignment) : VertexFactory(),
		TextFont(font), Text(text), Color(color), HorizontalAlign(horizontalAlignment), VerticalAlign(verticalAlignment)
	{

	}

	ModelData TextFactory::GenerateVertices() const
	{
		std::vector<Font::FontCharacter> characters = TextFont->GetCharacters(Text);
		Vector2f size = TextFont->SizeOfText(Text);
		float height = TextFont->SizeOfText("fg").y;
		float w = size.x / 2.0f;
		float h = -height / 2.0f;

		ModelData result;
		result.Vertices = std::make_unique<VertexArray>();
		result.Indices.AddIndexBuffer(std::make_unique<IndexBuffer>(6 * characters.size()));
		result.Bounds.MinX = -w;
		result.Bounds.MaxX = w;
		result.Bounds.MinY = h;
		result.Bounds.MaxY = -h;
		result.Bounds.MinZ = 0;
		result.Bounds.MaxZ = 0;

		BufferLayout layout = BufferLayout::Default();
		result.Vertices->CreateVertexBuffer(4 * characters.size() * layout.Size(), layout);

		Vector4<byte> color = Color.ToBytes();

		if (Text.size() == 0)
		{
			return result;
		}

		{
			VertexIterator iterator = result.Vertices->Begin();
			IndexIterator<uint> indices = result.Indices.Begin();
			
			float currentX = -w;
			float currentY = h;
			switch (HorizontalAlign)
			{
			case AlignH::Left:
				currentX = 0.0f;
				break;
			case AlignH::Right:
				currentX = -w * 2.0f;
				break;
			}
			switch (VerticalAlign)
			{
			case AlignV::Bottom:
				currentY = 0.0f;
				break;
			case AlignV::Top:
				currentY = h * 2.0f;
				break;
			}
			for (int i = 0; i < characters.size(); i++)
			{				
				Font::FontCharacter& chr = characters[i];
				float x = currentX + chr.OffsetX + chr.Kerning;
				float y = currentY + chr.OffsetY;
				iterator[0] = Vector3f(x, y, 0);
				iterator[1] = Vector3f(0.0f, 0.0f, 1.0f);
				iterator[2] = Vector2f(chr.Bounds.MinX, chr.Bounds.MaxY);
				iterator[3] = color;
				iterator++;
				iterator[0] = Vector3f(x, y - chr.Height, 0);
				iterator[1] = Vector3f(0.0f, 0.0f, 1.0f);
				iterator[2] = Vector2f(chr.Bounds.MinX, chr.Bounds.MinY);
				iterator[3] = color;
				iterator++;
				iterator[0] = Vector3f(x + chr.Width, y - chr.Height, 0);
				iterator[1] = Vector3f(0.0f, 0.0f, 1.0f);
				iterator[2] = Vector2f(chr.Bounds.MaxX, chr.Bounds.MinY);
				iterator[3] = color;
				iterator++;
				iterator[0] = Vector3f(x + chr.Width, y, 0);
				iterator[1] = Vector3f(0.0f, 0.0f, 1.0f);
				iterator[2] = Vector2f(chr.Bounds.MaxX, chr.Bounds.MaxY);
				iterator[3] = color;
				iterator++;

				*indices = i * 4 + 0;
				indices++;
				*indices = i * 4 + 1;
				indices++;
				*indices = i * 4 + 2;
				indices++;
				*indices = i * 4 + 0;
				indices++;
				*indices = i * 4 + 2;
				indices++;
				*indices = i * 4 + 3;
				indices++;

				currentX += chr.AdvanceX + chr.Kerning;
				currentY += chr.AdvanceY;
			}
		}

		return result;
	}

}