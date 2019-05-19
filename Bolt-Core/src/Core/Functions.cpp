#include "bltpch.h"

#include "Functions.h"
#include "Utils/Filesystem/Filesystem.h"
#define STB_IMAGE_IMPLEMENTATION 
#include "../vendor/stb_image.h"

namespace Bolt
{

	Image LoadTexture(const Filepath& file, bool flipVertically, bool flipHorizontally)
	{
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