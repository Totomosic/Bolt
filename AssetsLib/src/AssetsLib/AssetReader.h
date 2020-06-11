#pragma once
#include "AssetHeader.h"
#include "Image2D.h"
#include "BoltLib/IO/Filepath.h"

namespace Bolt::Assets
{

	class BLT_API AssetReader
	{
	private:
		std::unique_ptr<uint8_t[]> m_Data;
		size_t m_Size;
		AssetHeader m_Header;

	public:
		AssetReader(const FilePath& assetFile);
		AssetReader(const void* data, size_t size);
		AssetReader(std::unique_ptr<uint8_t[]>&& data, size_t size);

		const AssetHeader& GetHeader() const;
		AssetType GetType() const;

		Asset<Image2D> ReadAsImage2D() const;

	private:
		void ReadHeader();
	};

}