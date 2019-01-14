#pragma once
#include "Types.h"
#include "Image.h"
#include "Utils\Filesystem\Filepath.h"

namespace Bolt
{

	template<typename T>
	T Map(T value, T origMin, T origMax, T newMin, T newMax)
	{
		return (T)(((value - origMin) / (origMax - origMin)) * (newMax - newMin) + newMin);
	}

	template<typename T>
	T Clamp(T value, T min, const T& max)
	{
		return (value > max) ? max : (value < min) ? min : value;
	}

	template<typename T, typename Time>
	T Lerp(T a, T b, Time amount)
	{
		return (T)(a + (b - a) * amount);
	}

	template<typename T, typename Other>
	T explicit_cast(Other t)
	{
		return t.operator T();
	}

	Image LoadTexture(const Filepath& file, bool flipVertically = true, bool flipHorizontally = false);

}