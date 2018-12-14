#pragma once
#include "Bolt-Graphics.h"

namespace Bolt
{

	class BLT_API GLState
	{
	private:
		static bool s_DepthEnabled;
		static bool s_BlendEnabled;
		static bool s_CullFaceEnabled;
		static DepthFunction s_DepthFunction;
		static BlendSrc s_BlendSrc;
		static BlendDst s_BlendDst;
		static CullFace s_CullFace;
		static PolygonMode s_PolygonMode;
		static bool s_RenderToDepthBuffer;

	public:
		GLState() = delete;

		static void Reset();

		static void SetDepthTest(bool enabled);
		static void SetBlending(bool enabled);
		static void SetFaceCulling(bool enabled);
		static void SetClippingPlane(id_t planeId, bool enabled);

		static void SetDepthFunc(DepthFunction func);
		static void SetBlendFunc(BlendSrc src, BlendDst dst);
		static void SetBlendSrc(BlendSrc src);
		static void SetBlendDst(BlendDst dst);
		static void SetCullFace(CullFace face);
		static void SetPolygonMode(PolygonMode mode);
		static void SetRenderToDepthBuffer(bool renderToDepth);

		static void ApplySettings(const RenderSettings& settings);

	};

}