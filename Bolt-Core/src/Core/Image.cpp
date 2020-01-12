#include "bltpch.h"
#include "Image.h"
#include "BoltLib/Functions.h"
#include "BoltLib/Profiling/Profiling.h"
#include "Core/Utils/Filesystem/Filepath.h"
#include "Core/Utils/Filesystem/Filesystem.h"

#define STB_IMAGE_IMPLEMENTATION 
#include "../vendor/stb_image.h"

namespace Bolt
{

	Image::Image()
		: Width(0), Height(0), Components(0), Pixels(nullptr)
	{

	}

	Image::Image(int width, int height, int components, byte* pixels)
		: Width(width), Height(height), Components(components), Pixels(pixels)
	{
	
	}

	Image::Image(Image&& other)
		: Width(other.Width), Height(other.Height), Components(other.Components), Pixels(other.Pixels)
	{
		other.Pixels = nullptr;
	}

	Image& Image::operator=(Image&& other)
	{
		Width = other.Width;
		Height = other.Height;
		Components = other.Components;
		byte* myPixels = Pixels;
		Pixels = other.Pixels;
		other.Pixels = myPixels;
		return *this;
	}

	Image::~Image()
	{
		BLT_PROFILE_FUNCTION();
		if (Pixels != nullptr)
		{
			BLT_DELETE_ARR Pixels;
		}
	}

	Image Image::Resize(int width, int height, ResizeFilter filter) const
	{
		BLT_PROFILE_FUNCTION();
		switch (filter)
		{
		case ResizeFilter::Linear:
			return ResizeLinear(width, height);
		case ResizeFilter::Nearest:
			return ResizeNearest(width, height);
		default:
			return ResizeNearest(width, height);
		}
	}

	byte* Image::ReleasePixels()
	{
		BLT_PROFILE_FUNCTION();
		byte* pixelsPtr = Pixels;
		Pixels = nullptr;
		return pixelsPtr;
	}

	Image Image::ResizeLinear(int width, int height) const
	{
		BLT_PROFILE_FUNCTION();
		Image result;
		result.Width = width;
		result.Height = height;
		result.Components = Components;
		result.Pixels = BLT_NEW byte[(int64_t)width * (int64_t)height * (int64_t)Components];

		for (int i = 0; i < result.Width; i++)
		{
			float oldWidth = Map<float>((float)i, 0, (float)result.Width, 0, (float)Width);
			int w = (int)oldWidth;
			float wAmount = oldWidth - w;
			for (int j = 0; j < result.Height; j++)
			{
				float oldHeight = Map<float>((float)j, 0, (float)result.Height, 0, (float)Height);
				int h = (int)oldHeight;
				float hAmount = oldHeight - h;

				byte* tl = GetPixel(w, h);
				byte* tr = GetPixel(w + 1, h);
				byte* bl = GetPixel(w, h + 1);
				byte* br = GetPixel(w + 1, h + 1);
				for (int k = 0; k < result.Components; k++)
				{
					result.Pixels[(i + j * result.Width) * result.Components + k] = Lerp(Lerp(tl[k], tr[k], wAmount), Lerp(bl[k], br[k], wAmount), hAmount);
				}
			}
		}

		return result;
	}

	Image Image::ResizeNearest(int width, int height) const
	{
		BLT_PROFILE_FUNCTION();
		Image result;
		result.Width = width;
		result.Height = height;
		result.Components = Components;
		result.Pixels = BLT_NEW byte[width * height * Components];

		for (int i = 0; i < result.Width; i++)
		{
			int oldWidth = (int)Map<float>((float)i, 0, (float)result.Width - 1, 0, (float)Width - 1);
			for (int j = 0; j < result.Height; j++)
			{
				int oldHeight = (int)Map<float>((float)j, 0, (float)result.Height - 1, 0, (float)Height - 1);
				byte* pixel = GetPixel(oldWidth, oldHeight);
				for (int k = 0; k < result.Components; k++)
				{
					result.Pixels[(i + j * result.Width) * result.Components + k] = pixel[k];
				}
			}
		}
		return result;
	}

	byte* Image::GetPixel(int x, int y) const
	{
		return Pixels + (uintptr_t)((x + y * Width) * Components);
	}

	Image LoadTexture(const FilePath& file, bool flipVertically, bool flipHorizontally)
	{
		BLT_PROFILE_FUNCTION();
		BLT_ASSERT(Filesystem::FileExists(file), "Unable to find Image with path " + file.Path());
		Image result;
		int channels;
		stbi_set_flip_vertically_on_load(flipVertically);
		result.Components = 4;
		result.Pixels = stbi_load(file.Path().c_str(), &result.Width, &result.Height, &channels, 4);
		if (flipHorizontally)
		{
			for (int y = 0; y < result.Height; y++)
			{
				for (int x = 0; x < result.Width / 2; x++)
				{
					byte* pixel = result.GetPixel(x, y);
					byte r = pixel[0];
					byte g = pixel[1];
					byte b = pixel[2];
					byte a = pixel[3];
					byte* otherPixel = result.GetPixel(result.Width - x - 1, y);
					pixel[0] = otherPixel[0];
					pixel[1] = otherPixel[1];
					pixel[2] = otherPixel[2];
					pixel[3] = otherPixel[3];
					otherPixel[0] = r;
					otherPixel[1] = g;
					otherPixel[2] = b;
					otherPixel[3] = a;
				}
			}
		}
		return result;
	}

}