#include "LineFactory.h"

namespace Bolt
{

	LineFactory::LineFactory(const Vector3f& direction, float length, const Bolt::Color& color) : VertexFactory(),
		Direction(direction), Length(length), Color(color)
	{

	}

	ModelData LineFactory::GenerateVertices() const
	{
		ModelData result;
		result.Vertices = std::make_unique<VertexArray>(RenderMode::Lines);
		uint indices[2] = { 0, 1 };
		result.Indices.AddIndexBuffer(std::make_unique<IndexBuffer>(indices, 2));

		BufferLayout layout = BufferLayout::Default();
		result.Vertices->CreateVertexBuffer(2 * layout.Size(), layout);

		Vector4<byte> color = Color.ToBytes();

		VertexIterator iterator = result.Vertices->Begin();
		iterator[0] = -Direction * Length / 2.0f;
		iterator[1] = Vector3f(0, 0, 1);
		iterator[2] = Vector2f(0, 1);
		iterator[3] = color;
		iterator++;
		iterator[0] = Direction * Length / 2.0f;
		iterator[1] = Vector3f(0, 0, 1);
		iterator[2] = Vector2f(0, 0);
		iterator[3] = color;

		return result;
	}

}