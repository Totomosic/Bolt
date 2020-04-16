#pragma once
#include "IndexBuffer.h"
#include "IArrayDescriptor.h"
#include "Core/Tasks/TaskManager.h"

namespace Bolt
{

	class BLT_API IndexArray
	{
	private:
		std::vector<std::unique_ptr<IndexBuffer>> m_IndexBuffers;
		IArrayDescriptor m_Descriptor;

	public:
		IndexArray();
		IndexArray(const IndexArray& other) = delete;
		IndexArray& operator=(const IndexArray& other) = delete;
		IndexArray(IndexArray&& other) noexcept;
		IndexArray& operator=(IndexArray&& other) noexcept;
		~IndexArray();

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
		std::unique_ptr<IndexArray> Clone() const;

		friend class IndexMapping;
	};

}