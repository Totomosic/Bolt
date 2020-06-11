#include "AssetReader.h"
#include "BoltLib/IO/Filesystem.h"

namespace Bolt::Assets
{

	AssetReader::AssetReader(const FilePath& assetFile)
		: m_Data(nullptr), m_Size(0), m_Header()
	{
		BLT_ASSERT(Filesystem::FileExists(assetFile), "Asset {} does not exist", assetFile);
		File f = Filesystem::Open(assetFile, OpenMode::Read);
		size_t fileSize = f.GetSize();

		m_Size = fileSize;
		m_Data = std::make_unique<uint8_t[]>(m_Size);
		f.Read(m_Data.get(), m_Size);
		ReadHeader();
	}

	AssetReader::AssetReader(const void* data, size_t size)
		: m_Data(std::make_unique<uint8_t[]>(size)), m_Size(size), m_Header()
	{
		memcpy(m_Data.get(), data, size);
		ReadHeader();
	}

	AssetReader::AssetReader(std::unique_ptr<uint8_t[]>&& data, size_t size)
		: m_Data(std::move(data)), m_Size(size), m_Header()
	{
		ReadHeader();
	}

	const AssetHeader& AssetReader::GetHeader() const
	{
		return m_Header;
	}

	AssetType AssetReader::GetType() const
	{
		return m_Header.Type;
	}

	Asset<Image2D> AssetReader::ReadAsImage2D() const
	{
		BLT_ASSERT(GetType() == AssetType::Image2D, "Invalid asset type - cannot load as Image2D");
		BLT_ASSERT(m_Header.MetadataLength == Image2DHeader::GetSize(), "Invalid asset file - Header length does not match");
		Asset<Image2D> result;
		result.Name = m_Header.Name;
		size_t headerOffset = AssetHeader::GetSize();
		Image2DHeader header = Image2DHeader::Deserialize(m_Data.get() + headerOffset);
		BLT_ASSERT(header.Width >= 0 && header.Height >= 0, "Invalid asset file - Negative image dimensions");
		size_t dataLength = (size_t)header.Width * (size_t)header.Height * (size_t)GetComponentCount(header.Format);
		BLT_ASSERT(m_Size == headerOffset + m_Header.MetadataLength + dataLength, "Invalid asset file - size mismatch");
		result.Data.Width = header.Width;
		result.Data.Height = header.Height;
		result.Data.Format = header.Format;
		result.Data.Parameters = header.Parameters;
		result.Data.Pixels = std::shared_ptr<uint8_t>(new uint8_t[dataLength]);
		memcpy(result.Data.Pixels.get(), m_Data.get() + headerOffset + m_Header.MetadataLength, dataLength);
		return result;
	}

	void AssetReader::ReadHeader()
	{
		BLT_ASSERT(m_Size >= AssetHeader::GetSize(), "Invalid Asset File - Does not contain header");
		m_Header = AssetHeader::Deserialize(m_Data.get());
		BLT_ASSERT(m_Size >= AssetHeader::GetSize() + m_Header.MetadataLength, "Invalid Asset File - Does not contain metadata");
	}

}
