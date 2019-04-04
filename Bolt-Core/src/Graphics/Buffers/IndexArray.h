#pragma once
#include "IndexBuffer.h"
#include "IArrayDescriptor.h"
#include "IndexMapping.h"

namespace Bolt
{

	class BLT_API IndexArray
	{
	private:
		std::vector<std::unique_ptr<IndexBuffer>> m_IndexBuffers;
		IArrayDescriptor m_Descriptor;
		mutable bool m_IsMapped;

	public:
		IndexArray();
		IndexArray(const IndexArray& other) = delete;
		IndexArray& operator=(const IndexArray& other) = delete;
		IndexArray(IndexArray&& other) noexcept;
		IndexArray& operator=(IndexArray&& other) noexcept;
		~IndexArray() = default;

		const std::vector<std::unique_ptr<IndexBuffer>>& GetIndexBuffers() const;
		int IndexBufferCount() const;
		int IndexCount() const;
		const IArrayDescriptor& Descriptor() const;
		const std::unique_ptr<IndexBuffer>& GetIndexBuffer(int index) const;
		std::unique_ptr<IndexBuffer>& GetIndexBuffer(int index);
		const std::unique_ptr<IndexBuffer>& operator[](int index) const;
		std::unique_ptr<IndexBuffer>& operator[](int index);
		bool IsMapped() const;

		IndexBuffer& AddIndexBuffer(std::unique_ptr<IndexBuffer>&& buffer);
		IndexMapping Map() const;

		IndexArray Clone() const;

		friend class IndexMapping;

	private:
		void SetMapped(bool isMapped) const;

	};

}