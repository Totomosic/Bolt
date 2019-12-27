#pragma once
#include "Graphics/Resources/ResourcePtr.h"
#include "Graphics/Resources/Textures/Texture2D.h"

namespace Bolt
{

	BLT_API enum class ShaderStage
	{
		Vertex,
		Geometry,
		Fragment,
		Ignore
	};

	BLT_API enum class ValueType
	{
		Void,
		Bool,
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

	BLT_API enum class ValueTypeDim
	{
		Single,
		Array
	};

	struct BLT_API ValueTypeInfo
	{
	public:
		ValueType Type = ValueType::Void;
		ValueTypeDim Dimension = ValueTypeDim::Single;
	};

	BLT_API enum class RendererUniform
	{
		ModelMatrix,
		ViewMatrix,
		ProjectionMatrix,
		NormalMatrix,
		Time,
		CameraPosition,
		CameraDirection,
		LightCount,
		LightPositions,
		LightTypes,
		LightColors,
		LightDirections,
		LightAmbients,
		LightAmbientColors,
		LightAttenuations,
		LightIntensities
	};

	BLT_API enum class PassType
	{
		None,
		Flat,
		Varying
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
		case RendererUniform::NormalMatrix:
			return ValueType::Matrix4f;
		case RendererUniform::Time:
			return ValueType::Float;
		case RendererUniform::CameraPosition:
			return ValueType::Vector3f;
		case RendererUniform::CameraDirection:
			return ValueType::Vector3f;
		case RendererUniform::LightCount:
			return ValueType::Int;
		case RendererUniform::LightPositions:
			return ValueType::Vector3f;
		case RendererUniform::LightTypes:
			return ValueType::Int;
		case RendererUniform::LightColors:
			return ValueType::Vector4f;
		case RendererUniform::LightDirections:
			return ValueType::Vector3f;
		case RendererUniform::LightAmbients:
			return ValueType::Float;
		case RendererUniform::LightAmbientColors:
			return ValueType::Vector4f;
		case RendererUniform::LightAttenuations:
			return ValueType::Vector3f;
		case RendererUniform::LightIntensities:
			return ValueType::Float;
		}
		BLT_ASSERT(false, "Unable to determine uniform type");
		return ValueType::Void;
	}

	inline ValueTypeDim GetTypeDimOfRendererUniform(RendererUniform uniform)
	{
		switch (uniform)
		{
		case RendererUniform::ModelMatrix:
			return ValueTypeDim::Single;
		case RendererUniform::ViewMatrix:
			return ValueTypeDim::Single;
		case RendererUniform::ProjectionMatrix:
			return ValueTypeDim::Single;
		case RendererUniform::NormalMatrix:
			return ValueTypeDim::Single;
		case RendererUniform::Time:
			return ValueTypeDim::Single;
		case RendererUniform::CameraPosition:
			return ValueTypeDim::Single;
		case RendererUniform::CameraDirection:
			return ValueTypeDim::Single;
		case RendererUniform::LightCount:
			return ValueTypeDim::Single;
		case RendererUniform::LightPositions:
			return ValueTypeDim::Array;
		case RendererUniform::LightTypes:
			return ValueTypeDim::Array;
		case RendererUniform::LightColors:
			return ValueTypeDim::Array;
		case RendererUniform::LightDirections:
			return ValueTypeDim::Array;
		case RendererUniform::LightAmbients:
			return ValueTypeDim::Array;
		case RendererUniform::LightAmbientColors:
			return ValueTypeDim::Array;
		case RendererUniform::LightAttenuations:
			return ValueTypeDim::Array;
		case RendererUniform::LightIntensities:
			return ValueTypeDim::Array;
		}
		BLT_ASSERT(false, "Unable to determine uniform dimension");
		return ValueTypeDim::Single;
	}

	inline int GetArrayLengthOfRendererUniform(RendererUniform uniform)
	{
		switch (uniform)
		{
		case RendererUniform::LightPositions:
			return 10;
		case RendererUniform::LightTypes:
			return 10;
		case RendererUniform::LightColors:
			return 10;
		case RendererUniform::LightDirections:
			return 10;
		case RendererUniform::LightAmbients:
			return 10;
		case RendererUniform::LightAmbientColors:
			return 10;
		case RendererUniform::LightIntensities:
			return 10;
		case RendererUniform::LightAttenuations:
			return 10;
		}
		BLT_ASSERT(false, "Unable to determine uniform length");
		return 0;
	}

#define BLT_VALUE_TYPE_TO_GLSL_STRING_HELPER(type, str)	\
	case ValueType::type:	\
		return str;

	inline blt::string ValueTypeToGLSLString(ValueType type)
	{
		switch (type)
		{
			BLT_VALUE_TYPE_TO_GLSL_STRING_HELPER(Void, "void")
			BLT_VALUE_TYPE_TO_GLSL_STRING_HELPER(Bool, "bool")
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
#undef BLT_VALUE_TYPE_TO_GLSL_STRING_HELPER

#define BLT_GLSL_STRING_TO_VALUE_TYPE_HELPER(type, str)	\
	if (glslStr == str) { return ValueType::type; }

	inline ValueType GLSLStringToValueType(const blt::string& glslStr)
	{
		BLT_GLSL_STRING_TO_VALUE_TYPE_HELPER(Void, "void");
		BLT_GLSL_STRING_TO_VALUE_TYPE_HELPER(Bool, "bool");
		BLT_GLSL_STRING_TO_VALUE_TYPE_HELPER(Int, "int");
		BLT_GLSL_STRING_TO_VALUE_TYPE_HELPER(Float, "float");

		BLT_GLSL_STRING_TO_VALUE_TYPE_HELPER(Vector2f, "vec2");
		BLT_GLSL_STRING_TO_VALUE_TYPE_HELPER(Vector3f, "vec3");
		BLT_GLSL_STRING_TO_VALUE_TYPE_HELPER(Vector4f, "vec4");

		BLT_GLSL_STRING_TO_VALUE_TYPE_HELPER(Matrix2f, "mat2");
		BLT_GLSL_STRING_TO_VALUE_TYPE_HELPER(Matrix3f, "mat3");
		BLT_GLSL_STRING_TO_VALUE_TYPE_HELPER(Matrix4f, "mat4");

		BLT_GLSL_STRING_TO_VALUE_TYPE_HELPER(Texture1D, "sampler1D");
		BLT_GLSL_STRING_TO_VALUE_TYPE_HELPER(Texture2D, "sampler2D");
		BLT_GLSL_STRING_TO_VALUE_TYPE_HELPER(Texture3D, "sampler3D");
		BLT_GLSL_STRING_TO_VALUE_TYPE_HELPER(TextureCube, "samplerCube");
		BLT_ASSERT(false, "Invalid glsl string");
		return ValueType::Void;
	}
#undef BLT_GLSL_STRING_TO_VALUE_TYPE_HELPER

#define BLT_RENDERER_UNIFORM_FROM_STRING_HELPER(uniform)	\
	if (str == #uniform) { return RendererUniform::uniform; }

	inline RendererUniform GetRendererUniformFromString(const blt::string& str)
	{
		BLT_RENDERER_UNIFORM_FROM_STRING_HELPER(ModelMatrix);
		BLT_RENDERER_UNIFORM_FROM_STRING_HELPER(ViewMatrix);
		BLT_RENDERER_UNIFORM_FROM_STRING_HELPER(ProjectionMatrix);
		BLT_RENDERER_UNIFORM_FROM_STRING_HELPER(Time);
		BLT_RENDERER_UNIFORM_FROM_STRING_HELPER(CameraPosition);
		BLT_RENDERER_UNIFORM_FROM_STRING_HELPER(CameraDirection);
		BLT_RENDERER_UNIFORM_FROM_STRING_HELPER(LightCount);
		BLT_RENDERER_UNIFORM_FROM_STRING_HELPER(LightPositions);
		BLT_RENDERER_UNIFORM_FROM_STRING_HELPER(LightTypes);
		BLT_RENDERER_UNIFORM_FROM_STRING_HELPER(LightColors);
		BLT_RENDERER_UNIFORM_FROM_STRING_HELPER(LightDirections);
		BLT_RENDERER_UNIFORM_FROM_STRING_HELPER(LightAmbients);
		BLT_RENDERER_UNIFORM_FROM_STRING_HELPER(LightAttenuations);
		BLT_RENDERER_UNIFORM_FROM_STRING_HELPER(LightIntensities);
		BLT_ASSERT(false, "Invalid renderer uniform");
		return RendererUniform::ModelMatrix;
	}
#undef BLT_RENDERER_UNIFORM_FROM_STRING_HELPER

	inline blt::string GetGLSLStringFromPassType(PassType type)
	{
		switch (type)
		{
		case PassType::None:
			return "";
		case PassType::Flat:
			return "flat";
		case PassType::Varying:
			return "varying";
		}
		BLT_ASSERT(false, "Invalid PassType {}", (int)type);
		return "";
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

	inline int ValueTypeGetDimension(ValueType type)
	{
		switch (type)
		{
		case ValueType::Bool:
			return 1;
		case ValueType::Int:
			return 1;
		case ValueType::Float:
			return 1;
		case ValueType::Vector2f:
			return 2;
		case ValueType::Vector3f:
			return 3;
		case ValueType::Vector4f:
			return 4;
		case ValueType::Matrix2f:
			return 4;
		case ValueType::Matrix3f:
			return 9;
		case ValueType::Matrix4f:
			return 16;
		}
		BLT_ASSERT(false, "Invalid ValueType");
		return 0;
	}

	template<typename T>
	inline ValueType GetValueType()
	{
		BLT_ASSERT(false, blt::string("Unable to determine type of ") + typeid(T).name());
		return ValueType::Void;
	}

	template<>
	inline ValueType GetValueType<bool>()
	{
		return ValueType::Bool;
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
	inline ValueType GetValueType<Color>()
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
	inline ValueType GetValueType<ResourcePtr<Texture2D>>()
	{
		return GetValueType<Texture2D>();
	}

}