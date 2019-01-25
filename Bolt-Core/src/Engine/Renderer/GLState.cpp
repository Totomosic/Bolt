#include "Types.h"

#include "GLState.h"

namespace Bolt
{

	bool GLState::s_DepthEnabled = false;
	bool GLState::s_BlendEnabled = false;
	bool GLState::s_CullFaceEnabled = false;

	DepthFunction GLState::s_DepthFunction = DepthFunction::Less;
	BlendSrc GLState::s_BlendSrc = BlendSrc::SrcAlpha;
	BlendDst GLState::s_BlendDst = BlendDst::OneMinusSrcAlpha;
	CullFace GLState::s_CullFace = CullFace::Back;
	PolygonMode GLState::s_PolygonMode = PolygonMode::Fill;
	bool GLState::s_RenderToDepthBuffer = true;

	void GLState::Reset()
	{
		s_DepthEnabled = true;
		s_BlendEnabled = true;
		s_CullFaceEnabled = true;
		s_DepthFunction = DepthFunction::Lequal;
		s_BlendSrc = BlendSrc::SrcAlpha;
		s_BlendDst = BlendDst::OneMinusSrcAlpha;
		s_CullFace = CullFace::Back;
		s_PolygonMode = PolygonMode::Fill;
		s_RenderToDepthBuffer = true;

		GL_CALL(glEnable(GL_DEPTH_TEST));
		GL_CALL(glEnable(GL_BLEND));
		GL_CALL(glEnable(GL_CULL_FACE));
		GL_CALL(glDepthFunc((GLenum)s_DepthFunction));
		GL_CALL(glBlendFunc((GLenum)s_BlendSrc, (GLenum)s_BlendDst));
		GL_CALL(glCullFace((GLenum)s_CullFace));
		GL_CALL(glPolygonMode(GL_FRONT_AND_BACK, (GLenum)s_PolygonMode));
		GL_CALL(glDepthMask(s_RenderToDepthBuffer));
	}

	void GLState::SetDepthTest(bool enabled)
	{
		if (s_DepthEnabled != enabled)
		{
			s_DepthEnabled = enabled;
			if (s_DepthEnabled)
			{
				GL_CALL(glEnable(GL_DEPTH_TEST));
			}
			else
			{
				GL_CALL(glDisable(GL_DEPTH_TEST));
			}
		}
	}

	void GLState::SetBlending(bool enabled)
	{
		if (s_BlendEnabled != enabled)
		{
			s_BlendEnabled = enabled;
			if (s_DepthEnabled)
			{
				GL_CALL(glEnable(GL_BLEND));
			}
			else
			{
				GL_CALL(glDisable(GL_BLEND));
			}
		}
	}

	void GLState::SetFaceCulling(bool enabled)
	{
		if (s_CullFaceEnabled != enabled)
		{
			s_CullFaceEnabled = enabled;
			if (s_CullFaceEnabled)
			{
				GL_CALL(glEnable(GL_CULL_FACE));
			}
			else
			{
				GL_CALL(glDisable(GL_CULL_FACE));
			}
		}
	}

	void GLState::SetClippingPlane(id_t planeId, bool enabled)
	{
		if (enabled)
		{
			GL_CALL(glEnable(GL_CLIP_DISTANCE0 + planeId));
		}
		else
		{
			GL_CALL(glDisable(GL_CLIP_DISTANCE0 + planeId));
		}
	}

	void GLState::SetDepthFunc(DepthFunction func)
	{
		if (s_DepthFunction != func)
		{
			s_DepthFunction = func;
			GL_CALL(glDepthFunc((GLenum)func));
		}
	}

	void GLState::SetBlendFunc(BlendSrc src, BlendDst dst)
	{
		if (s_BlendSrc != src || s_BlendDst != dst)
		{
			s_BlendSrc = src;
			s_BlendDst = dst;
			GL_CALL(glBlendFunc((GLenum)s_BlendSrc, (GLenum)s_BlendDst));
		}
	}

	void GLState::SetBlendSrc(BlendSrc src)
	{
		SetBlendFunc(src, s_BlendDst);
	}

	void GLState::SetBlendDst(BlendDst dst)
	{
		SetBlendFunc(s_BlendSrc, dst);
	}

	void GLState::SetCullFace(CullFace face)
	{
		if (s_CullFace != face)
		{
			s_CullFace = face;
			GL_CALL(glCullFace((GLenum)face));
		}
	}

	void GLState::SetPolygonMode(PolygonMode mode)
	{
		if (s_PolygonMode != mode)
		{
			s_PolygonMode = mode;
			GL_CALL(glPolygonMode(GL_FRONT_AND_BACK, (GLenum)s_PolygonMode));
		}
	}

	void GLState::SetRenderToDepthBuffer(bool renderToDepth)
	{
		if (s_RenderToDepthBuffer != renderToDepth)
		{
			s_RenderToDepthBuffer = renderToDepth;
			GL_CALL(glDepthMask(s_RenderToDepthBuffer));
		}
	}

	void GLState::ApplySettings(const RenderSettings& settings)
	{
		SetDepthTest(settings.UseDepthTest);
		SetBlending(settings.UseBlend);
		SetFaceCulling(settings.UseCullFace);
		SetDepthFunc(settings.DepthFunc);
		SetBlendFunc(settings.BlendSrcFunc, settings.BlendDstFunc);
		SetCullFace(settings.CulledFaces);
		SetPolygonMode(settings.PolygonMode);
		SetRenderToDepthBuffer(settings.RenderToDepthBuffer);
	}

}