#include "RectangleFactory.h"

namespace Bolt
{

	RectangleFactory::RectangleFactory(float width, float height, const Bolt::Color& color) : VertexFactory(),
		Width(width), Height(height), Color(color)
	{
		
	}

	ModelData RectangleFactory::GenerateVertices() const
	{
		float w = Width / 2.0f;
		float h = Height / 2.0f;

		ModelData result;
		result.Vertices = std::make_unique<VertexArray>();
		uint indices[6] = { 0, 1, 2, 0, 2, 3 };
		result.Indices.AddIndexBuffer(std::make_unique<IndexBuffer>(indices, 6));

		BufferLayout layout = BufferLayout::Default();
		result.Vertices->CreateVertexBuffer(4 * layout.Size(), layout);

		Vector4<byte> color = Color.ToBytes();

		VertexIterator iterator = result.Vertices->Begin();
		iterator[0] = Vector3f(-w, h, 0);
		iterator[1] = Vector3f(0, 0, 1);
		iterator[2] = Vector2f(0, 1);
		iterator[3] = color;
		iterator++;
		iterator[0] = Vector3f(-w, -h, 0);
		iterator[1] = Vector3f(0, 0, 1);
		iterator[2] = Vector2f(0, 0);
		iterator[3] = color;
		iterator++;
		iterator[0] = Vector3f(w, -h, 0);
		iterator[1] = Vector3f(0, 0, 1);
		iterator[2] = Vector2f(1, 0);
		iterator[3] = color;
		iterator++;
		iterator[0] = Vector3f(w, h, 0);
		iterator[1] = Vector3f(0, 0, 1);
		iterator[2] = Vector2f(1, 1);
		iterator[3] = color;

		return result;
	}

}