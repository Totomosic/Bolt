#pragma once
#include "Texture.h"
#include "TextureBounds.h"
#include "TextureCreateOptions.h"

namespace Bolt
{

	class BLT_API Texture2D : public Texture
	{
	public:
		Color* Pixels;

	public:
		Texture2D(int width, int height, TextureFormat format, TextureCreateOptions options = TextureCreateOptions());
		Texture2D(int width, int height, TextureCreateOptions options = TextureCreateOptions());
		Texture2D(const Image& image, TextureFormat format, TextureCreateOptions options = TextureCreateOptions());
		Texture2D(const Image& image, TextureCreateOptions options = TextureCreateOptions());
		virtual ~Texture2D() override;

		void LoadPixels();
		void UpdatePixels();

	protected:
		void SetImage(const Image& image) const;

	};

}