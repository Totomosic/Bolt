#include "bltpch.h"
#include "FontFactory.h"

namespace Bolt
{

	FontFactory::FontFactory(const Filepath& fontFile)
		: m_FontFile(fontFile)
	{
		BLT_ASSERT(Filesystem::FileExists(fontFile), "File does not exist: {}", fontFile.Path());
	}

	std::unique_ptr<Font> FontFactory::BuildFont(float size, int textureWidth, int textureHeight) const
	{
		return std::make_unique<Font>(m_FontFile, size, textureWidth, textureHeight);
	}

	std::unique_ptr<Resource> FontFactory::Clone() const
	{
		return std::make_unique<FontFactory>(*this);
	}

}