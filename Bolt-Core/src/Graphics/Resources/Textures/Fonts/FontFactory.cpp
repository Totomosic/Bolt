#include "bltpch.h"
#include "FontFactory.h"

namespace Bolt
{

	FontFactory::FontFactory(const FilePath& fontFile)
		: m_FontFile(fontFile)
	{
		
	}

	std::unique_ptr<Font> FontFactory::BuildFont(float size, int textureWidth, int textureHeight) const
	{
		if (Filesystem::FileExists(m_FontFile))
		{
			return std::make_unique<Font>(m_FontFile, size, textureWidth, textureHeight);
		}
		BLT_CORE_WARN("Font file {} does not exist", m_FontFile);
		return nullptr;
	}

	std::unique_ptr<Resource> FontFactory::Clone() const
	{
		return std::make_unique<FontFactory>(*this);
	}

}