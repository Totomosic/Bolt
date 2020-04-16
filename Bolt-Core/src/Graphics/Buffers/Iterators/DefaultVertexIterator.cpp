#include "bltpch.h"
#include "DefaultVertexIterator.h"

namespace Bolt
{

	DefaultVertexIterator::DefaultVertexIterator() : VertexIterator()
	{
	}

	DefaultVertexIterator::DefaultVertexIterator(void* buffer, const BufferLayout* layout) : VertexIterator(buffer, layout)
	{
		BLT_ASSERT(*layout == BufferLayout::Default(), "Invalid buffer layout for default iterator");
	}

	DefaultVertexIterator::DefaultVertexIterator(const VertexIterator& it) : VertexIterator(it)
	{
		BLT_ASSERT(*m_Layout == BufferLayout::Default(), "Invalid buffer layout for default iterator");
	}

	Vector3f& DefaultVertexIterator::Position()
	{
		return GetSetter(BufferLayout::DefaultIndices.Position).Read<Vector3f>();
	}

	Vector3f& DefaultVertexIterator::Normal()
	{
		return GetSetter(BufferLayout::DefaultIndices.Normal).Read<Vector3f>();
	}

	Vector2f& DefaultVertexIterator::TexCoord()
	{
		return GetSetter(BufferLayout::DefaultIndices.TexCoord).Read<Vector2f>();
	}

	Vector4<byte>& DefaultVertexIterator::Color()
	{
		return GetSetter(BufferLayout::DefaultIndices.Color).Read<Vector4<byte>>();
	}

	Vector3f& DefaultVertexIterator::Tangent()
	{
		return GetSetter(BufferLayout::DefaultIndices.Tangent).Read<Vector3f>();
	}

}
