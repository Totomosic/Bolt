#pragma once
#include "Bolt-Core.h"

namespace Bolt
{

	BLT_API enum class DepthFunction : GLenum
	{
		Less = GL_LESS,
		Lequal = GL_LEQUAL,
		Equal = GL_EQUAL,
		Gequal = GL_GEQUAL,
		Greater = GL_GREATER
	};

	BLT_API enum class CullFace : GLenum
	{
		Front = GL_FRONT,
		Back = GL_BACK,
		FrontAndBack = GL_FRONT_AND_BACK
	};

	BLT_API enum class BlendSrc : GLenum
	{
		SrcAlpha = GL_SRC_ALPHA,
		One = GL_ONE
	};

	BLT_API enum class BlendDst : GLenum
	{
		OneMinusSrcAlpha = GL_ONE_MINUS_SRC_ALPHA,
		One = GL_ONE
	};

	BLT_API enum class PolygonMode : GLenum
	{
		Fill = GL_FILL,
		Line = GL_LINE,
		Point = GL_POINT
	};

	struct BLT_API RenderSettings
	{
	public:
		bool UseDepthTest = true;
		bool UseBlend = true;
		bool UseCullFace = true;
		DepthFunction DepthFunc = DepthFunction::Less;
		BlendSrc BlendSrcFunc = BlendSrc::SrcAlpha;
		BlendDst BlendDstFunc = BlendDst::OneMinusSrcAlpha;
		CullFace CulledFaces = CullFace::Back;
		PolygonMode PolygonMode = PolygonMode::Fill;
		bool RenderToDepthBuffer = true;

	public:
		bool operator==(const RenderSettings& other) const;
		bool operator!=(const RenderSettings& other) const;

	};

}