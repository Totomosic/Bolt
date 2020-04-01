#pragma once
#include "IndexBuffer.h"
#include "IArrayDescriptor.h"
#include "IndexMapping.h"
#include "Core/Tasks/TaskManager.h"

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
		IndexMapping Map() const;

		template<typename FuncT0>
		void MapAsync(FuncT0 callback)
		{
			Task t = TaskManager::Get().Run(make_shared_function([mapping{ Map() }, callback{ std::move(callback) }]() mutable
			{
				callback(mapping);
				return std::move(mapping);
			}));
			t.ContinueWithOnMainThread([](IndexMapping mapping)
			{
				
			});
		}

		std::unique_ptr<IndexArray> Clone() const;

		friend class IndexMapping;

	private:
		void SetMapped(bool isMapped) const;

	};

}