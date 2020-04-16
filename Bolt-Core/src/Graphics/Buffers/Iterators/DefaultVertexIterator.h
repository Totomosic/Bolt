#pragma once
#include "VertexIterator.h"

namespace Bolt
{

	class BLT_API DefaultVertexIterator : public VertexIterator
	{
	public:
		DefaultVertexIterator();
		DefaultVertexIterator(void* buffer, const BufferLayout* layout);
		DefaultVertexIterator(const VertexIterator& it);

		Vector3f& Position();
		Vector3f& Normal();
		Vector2f& TexCoord();
		Vector4<byte>& Color();
		Vector3f& Tangent();
	};

}