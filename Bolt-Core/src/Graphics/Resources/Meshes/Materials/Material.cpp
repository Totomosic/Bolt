#include "Types.h"

#include "Material.h"

namespace Bolt
{

	bool Material::operator==(const Material& other) const
	{
		return (BaseColor == other.BaseColor && RenderOptions == other.RenderOptions && Textures == other.Textures && LightingOptions == other.LightingOptions && Shader.Get() == other.Shader.Get());
	}

	bool Material::operator!=(const Material& other) const
	{
		return !(*this == other);
	}

	bool Material::HasTransparency() const
	{
		if (!RenderOptions.UseBlend)
		{
			return false;
		}
		if (BaseColor.a != 1.0f)
		{
			return true;
		}
		if (Textures.Textures.size() > 0)
		{
			return true;
		}
		return false;
	}

}