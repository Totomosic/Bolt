#include "bltpch.h"
#include "RenderTexture2D.h"

namespace Bolt
{

	RenderTexture2D::RenderTexture2D(int width, int height, TextureComponent component) : Texture2D(width, height, PixelFormat::RGBA, { PixelWrap::Repeat, PixelFilter::Linear, PixelFilter::Linear, false }), Framebuffer(width, height, 0, false)
	{
		if (component == TextureComponent::Color)
		{
			CreateColorBuffer(this, ColorBuffer::Color0);
			CreateDepthRenderBuffer();
		}
		else
		{
			CreateColorRenderBuffer(ColorBuffer::Color0);
			CreateDepthBuffer(this);
		}
	}

	id_t RenderTexture2D::TextureId() const
	{
		return Texture2D::Id();
	}

	id_t RenderTexture2D::FramebufferId() const
	{
		return Framebuffer::Id();
	}

}