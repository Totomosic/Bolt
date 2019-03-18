#pragma once
#include "ShaderVariable.h"

namespace Bolt
{

	class ShaderStream;
	using ShaderStreamPtr = std::shared_ptr<ShaderStream>;

	class BLT_API ShaderStream : public ShaderVariable
	{
	private:
		int m_Stream;

	private:
		ShaderStream(ValueType type, int stream);

	public:
		static ShaderStreamPtr Position();
		static ShaderStreamPtr Normal();
		static ShaderStreamPtr TexCoord();
		static ShaderStreamPtr Color();
		static ShaderStreamPtr Tangent();

	};

}