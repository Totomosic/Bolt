#pragma once
#include "Graphics/Assets/Meshes/Materials/RenderSettings.h"

namespace Bolt
{

	class BLT_API GLState
	{
	private:
		bool s_DepthEnabled;
		bool s_BlendEnabled;
		bool s_CullFaceEnabled;
		bool s_RenderToDepthBuffer;
		DepthFunction s_DepthFunction;
		BlendSrc s_BlendSrc;
		BlendDst s_BlendDst;
		CullFace s_CullFace;
		PolygonMode s_PolygonMode;

	public:
		GLState();

		void Reset();

		void SetDepthTest(bool enabled);
		void SetBlending(bool enabled);
		void SetFaceCulling(bool enabled);
		void SetClippingPlane(id_t planeId, bool enabled);

		void SetDepthFunc(DepthFunction func);
		void SetBlendFunc(BlendSrc src, BlendDst dst);
		void SetBlendSrc(BlendSrc src);
		void SetBlendDst(BlendDst dst);
		void SetCullFace(CullFace face);
		void SetPolygonMode(PolygonMode mode);
		void SetRenderToDepthBuffer(bool renderToDepth);

		void ApplySettings(const RenderSettings& settings);

	};

}