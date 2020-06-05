#pragma once
#include "Texture.h"
#include "TextureBounds.h"
#include "BoltLib/Color.h"
#include "AssetsLib/Image2D.h"

namespace Bolt
{

	class BLT_API Texture2D : public Texture
	{
	public:
		mutable Color* Pixels;

	public:
		Texture2D(int width, int height, PixelFormat format, Image2D::Options options = {});
		Texture2D(int width, int height, Image2D::Options options = {});
		Texture2D(int width, int height, const void* pixels, PixelFormat format, Image2D::Options options = {});
		Texture2D(const Image2D& image);
		Texture2D(Texture2D&& other);
		Texture2D& operator=(Texture2D&& other);
		virtual ~Texture2D() override;

		void LoadPixels() const;
		void UpdatePixels(bool deletePixels = true) const;
		Image2D GetImage() const;
		Image2D GetImage(int x, int y, int w, int h) const;

		void SetPixel(int x, int y, const Color& color);
		void SetRegion(int x, int y, int w, int h, const Color& color);
		void SetRegion(int x, int y, int w, int h, const Image2D& image);

	protected:
		void SetImage(const void* pixels) const;
		bool IsValidXY(int x, int y) const;

	};

}