#include "Types.h"
#include "ShaderStream.h"
#include "../ShaderBuilder.h"

namespace Bolt
{

	ShaderStream::ShaderStream(ValueType type, int stream) : ShaderVariable(type),
		m_Stream(stream)
	{
		
	}

	void ShaderStream::Build(ShaderBuilder& builder) const
	{
		if (!builder.IsDeclared(this))
		{
			m_Name = builder.GetStreamName(m_Stream);
			builder.WriteStreamLine("layout(location = " + std::to_string(m_Stream) + ") in " + ValueTypeToGLSLString(Type()) + ' ' + m_Name);
			builder.DeclareVariable(this);
		}
		builder.Write(m_Name);
	}

	ShaderStreamPtr ShaderStream::Position()
	{
		return std::make_shared<ShaderStream>(ValueType::Vector4f, POSITION_INDEX);
	}

	ShaderStreamPtr ShaderStream::Normal()
	{
		return std::make_shared<ShaderStream>(ValueType::Vector3f, NORMAL_INDEX);
	}

	ShaderStreamPtr ShaderStream::TexCoord()
	{
		return std::make_shared<ShaderStream>(ValueType::Vector2f, TEXCOORD_INDEX);
	}

	ShaderStreamPtr ShaderStream::Color()
	{
		return std::make_shared<ShaderStream>(ValueType::Vector4f, COLOR_INDEX);
	}

	ShaderStreamPtr ShaderStream::Tangent()
	{
		return std::make_shared<ShaderStream>(ValueType::Vector3f, TANGENT_INDEX);
	}

}