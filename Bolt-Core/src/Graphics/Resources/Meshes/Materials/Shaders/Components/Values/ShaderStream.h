#pragma once
#include "ShaderVariable.h"

namespace Bolt
{

	class ShaderStream;
	using ShaderStreamPtr = std::shared_ptr<ShaderStream>;

	class BLT_API ShaderStream : public ShaderVariable
	{
	public:
		static constexpr int POSITION_INDEX = 0;
		static constexpr int NORMAL_INDEX = 1;
		static constexpr int TEXCOORD_INDEX = 2;
		static constexpr int COLOR_INDEX = 3;
		static constexpr int TANGENT_INDEX = 4;

	private:
		int m_Stream;

	public:
		ShaderStream(ValueType type, int stream);

		virtual void Build(ShaderBuilder& builder) const override;

	public:
		static ShaderStreamPtr Position();
		static ShaderStreamPtr Normal();
		static ShaderStreamPtr TexCoord();
		static ShaderStreamPtr Color();
		static ShaderStreamPtr Tangent();

	};

}