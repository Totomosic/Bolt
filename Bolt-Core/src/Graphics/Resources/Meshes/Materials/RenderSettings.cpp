#include "Types.h"

#include "RenderSettings.h"

namespace Bolt
{

	bool RenderSettings::operator==(const RenderSettings& other) const
	{
		return (UseDepthTest == other.UseDepthTest && UseBlend == other.UseBlend && UseCullFace == other.UseCullFace &&
			DepthFunc == other.DepthFunc && BlendSrcFunc == other.BlendSrcFunc && BlendDstFunc == other.BlendDstFunc && CulledFaces == other.CulledFaces && PolygonMode == other.PolygonMode &&
			RenderToDepthBuffer == other.RenderToDepthBuffer);
	}

	bool RenderSettings::operator!=(const RenderSettings& other) const
	{
		return !(*this == other);
	}

}