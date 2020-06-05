#pragma once
#include "bltpch.h"
#include "Core/Utils/Filesystem/Filesystem.h"

namespace Bolt
{

	BLT_API enum class ResizeFilter
	{
		Linear,
		Nearest
	};

	struct BLT_API Image
	{
	public:
		int Width;
		int Height;
		int Components;
		byte* Pixels;

	public:
		Image();
		Image(int width, int height, int components, byte* pixels);
		Image(const Image& other) = delete;
		Image& operator=(const Image& other) = delete;
		Image(Image&& other);
		Image& operator=(Image&& other);
		~Image();

		Image Resize(int width, int height, ResizeFilter filter) const;
		byte* GetPixel(int x, int y) const;
		byte* ReleasePixels();

	private:
		Image ResizeNearest(int width, int height) const;
		Image ResizeLinear(int width, int height) const;		

	};

}