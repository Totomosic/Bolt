#pragma once
#include "Font.h"

namespace Bolt
{

	class BLT_API FontFactory : public Resource
	{
	private:
		byte* m_FontData;
		uint m_DataSize;

	public:
		FontFactory(const Filepath& fontFile);
		FontFactory(const byte* data, uint size);
		FontFactory(const FontFactory& other);
		FontFactory& operator=(const FontFactory& other);
		FontFactory(FontFactory&& other);
		FontFactory& operator=(FontFactory&& other);
		~FontFactory();

		std::unique_ptr<Font> BuildFont(float size, int textureWidth = 512, int textureHeight = 512) const;
		std::unique_ptr<Resource> Clone() const override;

	};

}