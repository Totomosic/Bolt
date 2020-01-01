#pragma once
#include "Font.h"

namespace Bolt
{

	class BLT_API FontFactory : public Resource
	{
	private:
		FilePath m_FontFile;

	public:
		FontFactory(const FilePath& fontFile);

		std::unique_ptr<Font> BuildFont(float size, int textureWidth = 512, int textureHeight = 512) const;
		std::unique_ptr<Resource> Clone() const override;

	};

}