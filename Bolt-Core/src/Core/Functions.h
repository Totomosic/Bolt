#pragma once
#include "Types.h"
#include "Image.h"
#include "Utils\Filesystem\Filepath.h"

namespace Bolt
{

	template<typename T>
	T Map(const T& value, const T& origMin, const T& origMax, const T& newMin, const T& newMax)
	{
		return (T)(((value - origMin) / (origMax - origMin)) * (newMax - newMin) + newMin);
	}

	template<typename T>
	T Clamp(const T& value, const T& min, const T& max)
	{
		return (value > max) ? max : (value < min) ? min : value;
	}

	template<typename T, typename Time>
	T Lerp(const T& a, const T& b, Time amount)
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