#include "Types.h"
#include "FontManager.h"
#include "ResourceManager.h"

namespace Bolt
{

	FontManager::FontManager()
		: m_ArialFactory("C:/Windows/Fonts/arial.ttf"), m_ArialFonts()
	{
		
	}

	ResourcePtr<const Font> FontManager::Arial(float fontSize) const
	{
		if (m_ArialFonts.find(fontSize) != m_ArialFonts.end())
		{
			return m_ArialFonts.at(fontSize);
		}
		std::unique_ptr<Font> arial = m_ArialFactory.BuildFont(fontSize);
		ResourcePtr<const Font> font = ResourceManager::Get<Font>(ResourceManager::Register(std::move(arial)));
		m_ArialFonts[fontSize] = font;
		return font;
	}

}