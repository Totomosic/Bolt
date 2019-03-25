#pragma once
#include "Bolt-Core.h"
#include "Graphics/Resources/ResourcePtr.h"
#include "Graphics/Resources/Textures/__Textures__.h"

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

	inline ValueType GetTypeOfRendererUniform(RendererUniform uniform)
	{
		switch (uniform)
		{
		case RendererUniform::ModelMatrix:
			return ValueType::Matrix4f;
		case RendererUniform::ViewMatrix:
			return ValueType::Matrix4f;
		case RendererUniform::ProjectionMatrix:
			return ValueType::Matrix4f;
		case RendererUniform::Time:
			return ValueType::Float;
		}
		BLT_ASSERT(false, "Unable to determine uniform type");
		return ValueType::Void;
	}

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

	inline bool ValueTypeIsNumeric(ValueType type)
	{
		return (type == ValueType::Float || type == ValueType::Int);
	}

	inline bool ValueTypeIsVector(ValueType type)
	{
		return (type == ValueType::Vector4f || type == ValueType::Vector3f || type == ValueType::Vector2f);
	}

	inline bool ValueTypeIsMatrix(ValueType type)
	{
		return (type == ValueType::Matrix4f || type == ValueType::Matrix3f || type == ValueType::Matrix2f);
	}

	inline bool ValueTypeIsTexture(ValueType type)
	{
		return (type == ValueType::Texture1D || type == ValueType::Texture2D || type == ValueType::Texture3D || type == ValueType::TextureCube);
	}

	template<typename T>
	inline ValueType GetValueType()
	{
		BLT_ASSERT(false, "Unable to determine type of " + typeid(T).name());
		return ValueType::Void;
	}

	template<>
	inline ValueType GetValueType<int>()
	{
		return ValueType::Int;
	}

	template<>
	inline ValueType GetValueType<float>()
	{
		return ValueType::Float;
	}

	template<>
	inline ValueType GetValueType<Vector2f>()
	{
		return ValueType::Vector2f;
	}

	template<>
	inline ValueType GetValueType<Vector3f>()
	{
		return ValueType::Vector3f;
	}

	template<>
	inline ValueType GetValueType<Vector4f>()
	{
		return ValueType::Vector4f;
	}

	template<>
	inline ValueType GetValueType<Matrix2f>()
	{
		return ValueType::Matrix2f;
	}

	template<>
	inline ValueType GetValueType<Matrix3f>()
	{
		return ValueType::Matrix3f;
	}

	template<>
	inline ValueType GetValueType<Matrix4f>()
	{
		return ValueType::Matrix4f;
	}

	template<>
	inline ValueType GetValueType<void>()
	{
		return ValueType::Void;
	}

	template<>
	inline ValueType GetValueType<Texture2D>()
	{
		return ValueType::Texture2D;
	}

	template<>
	inline ValueType GetValueType<const Texture2D>()
	{
		return GetValueType<Texture2D>();
	}

	template<>
	inline ValueType GetValueType<ResourcePtr<const Texture2D>>()
	{
		return GetValueType<Texture2D>();
	}

	template<>
	inline ValueType GetValueType<ResourcePtr<Texture2D>>()
	{
		return GetValueType<Texture2D>();
	}

}