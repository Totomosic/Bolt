#include "Types.h"
#include "Monitor.h"

namespace Bolt
{

	Monitor Monitor::Primary()
	{
		Monitor result;
		result.Handle = glfwGetPrimaryMonitor();
		result.VideoMode = glfwGetVideoMode(result.Handle);
		result.Width = result.VideoMode->width;
		result.Height = result.VideoMode->height;
		result.RedBits = result.VideoMode->redBits;
		result.GreenBits = result.VideoMode->greenBits;
		result.BlueBits = result.VideoMode->blueBits;
		result.AlphaBits = 8;
		return result;
	}

}