#pragma once
#include "Types.h"

namespace Bolt
{

	BLT_API enum class BufferUsage
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

	BLT_API enum class BufferTarget
	{
		ArrayBuffer = GL_ARRAY_BUFFER,
		ElementBuffer = GL_ELEMENT_ARRAY_BUFFER
	};

	BLT_API enum class Access
	{
		Read = GL_READ_ONLY,
		Write = GL_WRITE_ONLY,
		ReadWrite = GL_READ_WRITE
	};

	class BLT_API Buffer
	{
	protected:
		id_t m_Id;
		uint32_t m_Size;
		BufferUsage m_Usage;
		BufferTarget m_Target;
		mutable bool m_IsMapped;

	public:
		Buffer(uint32_t capacity, BufferTarget target, BufferUsage usage = BufferUsage::StaticDraw);
		Buffer(const void* data, uint32_t capacity, BufferTarget target, BufferUsage usage = BufferUsage::StaticDraw);		
		Buffer(Buffer&& other) noexcept;		
		Buffer& operator=(Buffer&& other) noexcept;
		virtual ~Buffer();
		Buffer(const Buffer& other) = delete;
		Buffer& operator=(const Buffer& other) = delete;

		uint32_t Size() const;
		BufferUsage Usage() const;
		BufferTarget Target() const;
		id_t Id() const;

		void Bind() const;
		void Unbind() const;

		virtual void* Map(Access access) const;
		virtual bool Unmap() const;

		void Upload(const void* data, uint32_t size, uint32_t offset = 0) const;
		void Download(void* outData, uint32_t size, uint32_t offset = 0) const;
		void Download(void* outData) const; // Download all

		// Resizes buffer and deletes contents
		void Resize(uint32_t newSize);
		void ResizePreserve(uint32_t newSize);
		void TestResize(uint32_t newSize);
		void TestResizePreserve(uint32_t newSize);
		void Append(const void* data, uint32_t size);
		void Insert(uint32_t position, const void* data, uint32_t size);
		void Erase(uint32_t position, uint32_t count);

	private:
		void Create(const void* data); // Can be nullptr

	};

}