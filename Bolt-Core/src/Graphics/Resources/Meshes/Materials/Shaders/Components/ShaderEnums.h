#pragma once
#include "Bolt-Core.h"

namespace Bolt
{

	BLT_API enum class ShaderType
	{
		Vertex,
		Geometry,
		Fragment,
		MAX_SHADER_TYPES,
		Ignore
	};

	BLT_API enum class ValueType
	{
		Void,
		Int,
		Float,

		Vector2i,
		Vector3i,
		Vector4i,
		Vector2f,
		Vector3f,
		Vector4f,

		Matrix2f,
		Matrix3f,
		Matrix4f,

		Texture1D,
		Texture2D,
		Texture3D,
		TextureCube
	};

	BLT_API enum class OutputValueType
	{
		Stream,
		RendererUniform,
		UserUniform,
		PassValue,
		Value,
		Constant,
		FunctionParam
	};

	BLT_API enum class ShaderStream
	{
		Position,
		Normal,
		TexCoord,
		Color,
		Tangent
	};

	BLT_API enum class RendererUniform
	{
		ModelMatrix,
		ViewMatrix,
		ProjectionMatrix,
		Time
	};

	BLT_API enum class ShaderAttribute
	{
		Position,
		FragColor
	};

}