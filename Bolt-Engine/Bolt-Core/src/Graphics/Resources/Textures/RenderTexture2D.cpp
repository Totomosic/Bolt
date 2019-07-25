#include "bltpch.h"
#include "RenderTexture2D.h"

namespace Bolt
{

	RenderTexture2D::RenderTexture2D(int width, int height, TextureComponent component) : Texture2D(width, height, TextureFormat::RGBA, { WrapMode::Repeat, MagFilter::Linear, MinFilter::Linear, Mipmaps::Disabled }), Framebuffer(width, height, 0, false)
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

}