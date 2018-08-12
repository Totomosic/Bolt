#pragma once
#include "Bolt-Core.h"
#include "../GLshared.h"

namespace Bolt
{

	enum class BufferUsage
	{
		StaticDraw = GL_STATIC_DRAW,
		StaticRead = GL_STATIC_READ,
		StaticCopy = GL_STATIC_COPY,
		DynamicDraw = GL_DYNAMIC_DRAW,
		DynamicRead = GL_DYNAMIC_READ,
		DynamicCopy = GL_DYNAMIC_COPY,
		StreamDraw = GL_STREAM_DRAW,
		StreamRead = GL_STREAM_READ,
		StreamCopy = GL_STREAM_COPY
	};

	enum class BufferTarget
	{
		ArrayBuffer = GL_ARRAY_BUFFER,
		ElementBuffer = GL_ELEMENT_ARRAY_BUFFER
	};

	enum class Access
	{
		Read = GL_READ_ONLY,
		Write = GL_WRITE_ONLY,
		ReadWrite = GL_READ_WRITE
	};

	class BLT_API Buffer : GLshared
	{
	protected:
		id_t m_Id;
		mutable size_t m_Size;
		size_t m_Capacity;
		BufferUsage m_Usage;
		BufferTarget m_Target;

		mutable void* m_MappedPtr;
		mutable Access m_MappedAccess;

	public:
		Buffer(size_t capacity, BufferTarget target, BufferUsage usage = BufferUsage::StaticDraw);
		Buffer(const void* data, size_t capacity, BufferTarget target, BufferUsage usage = BufferUsage::StaticDraw);		
		Buffer(Buffer&& other) noexcept;		
		Buffer& operator=(Buffer&& other) noexcept;
		virtual ~Buffer() override;
		Buffer(const Buffer& other) = delete;
		Buffer& operator=(const Buffer& other) = delete;

		size_t Size() const;
		size_t Capacity() const;
		BufferUsage Usage() const;
		BufferTarget Target() const;
		id_t ID() const;
		bool IsCurrentlyMapped() const;

		void Bind() const;
		void Unbind() const;

		virtual void* Map(Access access) const;
		virtual bool Unmap() const;

		void Upload(const void* data, size_t size, size_t offset = 0) const;
		void Download(void* outData, size_t size, size_t offset = 0) const;
		void Download(void* outData) const; // Download all

		// Resizes buffer and deletes contents
		void Resize(size_t newSize);
		void ResizePreserve(size_t newSize);
		void TestResize(size_t newSize);
		void TestResizePreserve(size_t newSize);
		void Append(const void* data, size_t size);
		void Insert(size_t position, const void* data, size_t size);
		void Erase(size_t position, size_t count);

	private:
		void Create(const void* data); // Can be nullptr

	};

}