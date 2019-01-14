#pragma once
#include "Texture.h"
#include "TextureBounds.h"
#include "TextureCreateOptions.h"

namespace Bolt
{

	class BLT_API Texture2D : public Texture
	{
	public:
		mutable Color* Pixels;

	public:
		Texture2D(int width, int height, TextureFormat format, TextureCreateOptions options = TextureCreateOptions());
		Texture2D(int width, int height, TextureCreateOptions options = TextureCreateOptions());
		Texture2D(const Image& image, TextureFormat format, TextureCreateOptions options = TextureCreateOptions());
		Texture2D(const Image& image, TextureCreateOptions options = TextureCreateOptions());
		Texture2D(Texture2D&& other);
		Texture2D& operator=(Texture2D&& other);
		virtual ~Texture2D() override;

		void LoadPixels() const;
		void UpdatePixels() const;
		Image GetImage() const;
		Image GetImage(int x, int y, int w, int h) const;

		void SetPixel(int x, int y, const Color& color);
		void SetRegion(int x, int y, int w, int h, const Color& color);
		void SetRegion(int x, int y, int w, int h, const Image& image);
		void SetRegion(int x, int y, int w, int h, const Image& image, ResizeFilter filter);
		void SetRegion(int x, int y, int w, int h, const Image& image, int ix, int iy, int iw, int ih, ResizeFilter filter);

	protected:
		void SetImage(const Image& image) const;
		bool IsValidXY(int x, int y) const;

	};

}