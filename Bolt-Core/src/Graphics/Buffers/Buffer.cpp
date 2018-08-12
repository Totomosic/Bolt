#include "Buffer.h"

namespace Bolt
{

	Buffer::Buffer(size_t capacity, BufferTarget target, BufferUsage usage) : Buffer(nullptr, capacity, target, usage)
	{
		
	}

	Buffer::Buffer(const void* data, size_t size, BufferTarget target, BufferUsage usage) : GLshared(),
		m_Id(0), m_Size(size), m_Capacity(size), m_Usage(usage), m_Target(target), m_MappedPtr(nullptr), m_MappedAccess(Access::Read)
	{
		Create(data);
	}

	Buffer::Buffer(Buffer&& other) noexcept
		: m_Id(other.m_Id), m_Size(other.m_Size), m_Capacity(other.m_Capacity), m_Target(other.m_Target), m_Usage(other.m_Usage), m_MappedPtr(nullptr), m_MappedAccess(Access::Read)
	{
		other.m_Id = 0;
	}

	Buffer& Buffer::operator=(Buffer&& other) noexcept
	{
		id_t tempId = m_Id;
		void* tempPtr = m_MappedPtr;
		Access tempAccess = m_MappedAccess;
		m_Id = other.m_Id;
		m_Size = other.m_Size;
		m_Capacity = other.m_Capacity;
		m_Target = other.m_Target;
		m_Usage = other.m_Usage;
		m_MappedPtr = other.m_MappedPtr;
		m_MappedAccess = other.m_MappedAccess;
		other.m_Id = tempId;
		other.m_MappedPtr = tempPtr;
		other.m_MappedAccess = tempAccess;
		return *this;
	}

	Buffer::~Buffer()
	{
		if (m_Id != 0)
		{
			GL_CALL(glDeleteBuffers(1, &m_Id));
		}
	}

	size_t Buffer::Size() const
	{
		return m_Size;
	}

	size_t Buffer::Capacity() const
	{
		return m_Capacity;
	}

	BufferUsage Buffer::Usage() const
	{
		return m_Usage;
	}

	BufferTarget Buffer::Target() const
	{
		return m_Target;
	}

	id_t Buffer::ID() const
	{
		return m_Id;
	}

	bool Buffer::IsCurrentlyMapped() const
	{
		return m_MappedPtr != nullptr;
	}

	void Buffer::Bind() const
	{
		GL_CALL(glBindBuffer((GLenum)m_Target, m_Id));
	}

	void Buffer::Unbind() const
	{
		GL_CALL(glBindBuffer((GLenum)m_Target, 0));
	}

	void* Buffer::Map(Access access) const
	{
		if (m_MappedPtr != nullptr && m_MappedAccess == access)
		{
			return m_MappedPtr;
		}
		else if (m_MappedPtr != nullptr)
		{
			Unmap();
		}
		Bind();
		void* result = GL_CALL(glMapBuffer((GLenum)m_Target, (GLenum)access));
		m_MappedPtr = result;
		m_MappedAccess = access;
		return result;
	}

	bool Buffer::Unmap() const
	{
		if (m_MappedPtr != nullptr)
		{
			Bind();
			bool result = GL_CALL(glUnmapBuffer((GLenum)m_Target));
			m_MappedPtr = nullptr;
			m_Size = m_Capacity; // Some way of testing this
			return result;
		}
		return true;
	}

	void Buffer::Upload(const void* data, size_t size, size_t offset) const
	{
		BLT_ASSERT(size + offset <= Capacity(), "Could not upload: " + std::to_string(size) + " as buffer is not large enough");
		BLT_ASSERT(!IsCurrentlyMapped(), "Buffer was mapped while trying to upload, ensure no active iterators are present.");
		Bind();
		GL_CALL(glBufferSubData((GLenum)m_Target, (GLintptr)offset, (GLsizeiptr)size, (const GLvoid*)data));
		m_Size += size;
	}

	void Buffer::Download(void* data, size_t size, size_t offset) const
	{
		BLT_ASSERT(size + offset <= Capacity(), "Could not download: " + std::to_string(size) + " as buffer does not contain that many bytes");
		BLT_ASSERT(!IsCurrentlyMapped(), "Buffer was mapped while trying to download, ensure no active iterators are present.");
		Bind();
		GL_CALL(glGetBufferSubData((GLenum)m_Target, (GLintptr)offset, (GLsizeiptr)size, (GLvoid*)data));
	}

	void Buffer::Download(void* data) const
	{
		Download(data, Size(), 0);
	}

	void Buffer::Resize(size_t newSize)
	{
		m_Capacity = newSize;
		if (m_Size > m_Capacity)
		{
			m_Size = m_Capacity;
		}
		GL_CALL(glBufferData((GLenum)m_Target, (GLsizeiptr)Capacity(), (const GLvoid*)nullptr, (GLenum)Usage()));
	}

	void Buffer::ResizePreserve(size_t newSize)
	{
		size_t oldSize = (newSize < Capacity()) ? newSize : Capacity();
		byte* oldData = new byte[oldSize];
		Download(oldData, oldSize, 0);
		Resize(newSize);
		Upload(oldData, oldSize, 0);
		delete[] oldData;
	}

	void Buffer::TestResize(size_t newSize)
	{
		if (newSize >= Capacity())
		{
			Resize(newSize);
		}
	}

	void Buffer::TestResizePreserve(size_t newSize)
	{
		if (newSize >= Capacity())
		{
			ResizePreserve(newSize);
		}
	}

	void Buffer::Append(const void* data, size_t size)
	{
		size_t oldSize = Size();
		TestResizePreserve(oldSize + size);
		Upload(data, size, oldSize);
		m_Size = oldSize + size;
	}

	void Buffer::Insert(size_t position, const void* data, size_t size)
	{
		size_t oldSize = Size();
		size_t rightDataSize = oldSize - position;
		byte* rightData = new byte[rightDataSize];
		Download(rightData, rightDataSize, position);
		TestResizePreserve(oldSize + size);
		Upload(data, size, position);
		Upload(rightData, rightDataSize, position + size);
		delete[] rightData;
		m_Size = oldSize + size;
	}

	void Buffer::Erase(size_t position, size_t count)
	{
		size_t oldSize = Size();
		size_t dataSize = oldSize - position - count;
		byte* data = new byte[dataSize];
		Download(data, dataSize, position + count);
		Upload(data, dataSize, position);
		delete[] data;
		m_Size = oldSize - count;
	}

	void Buffer::Create(const void* data)
	{
		GL_CALL(glGenBuffers(1, &m_Id));
		Bind();
		GL_CALL(glBufferData((GLenum)m_Target, (GLsizeiptr)Size(), (const GLvoid*)data, (GLenum)Usage()));
	}

}