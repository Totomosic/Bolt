#include "Types.h"
#include "FontFactory.h"

namespace Bolt
{

	FontFactory::FontFactory(const Filepath& fontFile)
	{
		BLT_ASSERT(Filesystem::FileExists(fontFile), "File does not exist: {}", fontFile.Path());
		File file = Filesystem::Open(fontFile, OpenMode::Read);
		uint fSize = file.GetSize();
		m_FontData = new byte[fSize];
		file.Read(m_FontData, fSize);
	}

	FontFactory::FontFactory(const byte* data, uint size)
		: m_FontData(new byte[size]), m_DataSize(size)
	{
		memcpy(m_FontData, data, size);
	}

	FontFactory::FontFactory(const FontFactory& other)
		: m_FontData(new byte[other.m_DataSize]), m_DataSize(other.m_DataSize)
	{
		memcpy(m_FontData, other.m_FontData, other.m_DataSize);
	}

	FontFactory& FontFactory::operator=(const FontFactory& other)
	{
		if (m_FontData != nullptr)
		{
			delete[] m_FontData;
		}
		m_FontData = new byte[other.m_DataSize];
		m_DataSize = other.m_DataSize;
		memcpy(m_FontData, other.m_FontData, other.m_DataSize);
		return *this;
	}

	FontFactory::FontFactory(FontFactory&& other)
		: m_FontData(other.m_FontData), m_DataSize(other.m_DataSize)
	{
		other.m_FontData = nullptr;
	}

	FontFactory& FontFactory::operator=(FontFactory&& other)
	{
		byte* data = m_FontData;
		m_FontData = other.m_FontData;
		m_DataSize = other.m_DataSize;
		other.m_FontData = data;
		return *this;
	}

	FontFactory::~FontFactory()
	{
		delete[] m_FontData;
	}

	std::unique_ptr<Font> FontFactory::BuildFont(float size, int textureWidth, int textureHeight) const
	{
		return std::make_unique<Font>(m_FontData, m_DataSize, size, textureWidth, textureHeight);
	}

	std::unique_ptr<Resource> FontFactory::Clone() const
	{
		return std::make_unique<FontFactory>(*this);
	}

}