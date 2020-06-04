#pragma once
#include "../../Framebuffer.h"
#include "Texture2D.h"

namespace Bolt
{

	BLT_API enum class TextureComponent
	{
		Color,
		Depth
	};

	class BLT_API RenderTexture2D : public Texture2D, public Framebuffer
	{
	public:
		RenderTexture2D(int width, int height, TextureComponent component = TextureComponent::Color);

		id_t TextureId() const;
		id_t FramebufferId() const;

	};

}