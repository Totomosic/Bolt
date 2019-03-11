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

	BLT_API enum class RendererUniform
	{
		ModelMatrix,
		ViewMatrix,
		ProjectionMatrix,
		Time
	};

#define BLT_VALUE_TYPE_TO_GLSL_STRING_HELPER(type, str)	\
	case ValueType::type:	\
		return str;

	inline blt::string ValueTypeToGLSLString(ValueType type)
	{
		switch (type)
		{
			BLT_VALUE_TYPE_TO_GLSL_STRING_HELPER(Void, "void")
			BLT_VALUE_TYPE_TO_GLSL_STRING_HELPER(Int, "int")
			BLT_VALUE_TYPE_TO_GLSL_STRING_HELPER(Float, "float")

			BLT_VALUE_TYPE_TO_GLSL_STRING_HELPER(Vector2i, "vec2")
			BLT_VALUE_TYPE_TO_GLSL_STRING_HELPER(Vector3i, "vec3")
			BLT_VALUE_TYPE_TO_GLSL_STRING_HELPER(Vector4i, "vec4")
			BLT_VALUE_TYPE_TO_GLSL_STRING_HELPER(Vector2f, "vec2")
			BLT_VALUE_TYPE_TO_GLSL_STRING_HELPER(Vector3f, "vec3")
			BLT_VALUE_TYPE_TO_GLSL_STRING_HELPER(Vector4f, "vec4")

			BLT_VALUE_TYPE_TO_GLSL_STRING_HELPER(Matrix2f, "mat2")
			BLT_VALUE_TYPE_TO_GLSL_STRING_HELPER(Matrix3f, "mat3")
			BLT_VALUE_TYPE_TO_GLSL_STRING_HELPER(Matrix4f, "mat4")

			BLT_VALUE_TYPE_TO_GLSL_STRING_HELPER(Texture1D, "sampler1D")
			BLT_VALUE_TYPE_TO_GLSL_STRING_HELPER(Texture2D, "sampler2D")
			BLT_VALUE_TYPE_TO_GLSL_STRING_HELPER(Texture3D, "sampler3D")
			BLT_VALUE_TYPE_TO_GLSL_STRING_HELPER(TextureCube, "samplerCube")
		}
		BLT_ASSERT(false, "Invalid type");
		return "ERROR_TYPE";
	}

}