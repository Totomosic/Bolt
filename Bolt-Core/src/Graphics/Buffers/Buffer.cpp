#include "Types.h"

#include "Buffer.h"

namespace Bolt
{

	Buffer::Buffer(size_t capacity, BufferTarget target, BufferUsage usage) : Buffer(nullptr, capacity, target, usage)
	{
		
	}

	Buffer::Buffer(const void* data, size_t size, BufferTarget target, BufferUsage usage)
		: m_Id(0), m_Size(size), m_Usage(usage), m_Target(target), m_IsMapped(false)
	{
		Create(data);
	}

	Buffer::Buffer(Buffer&& other) noexcept
		: m_Id(other.m_Id), m_Size(other.m_Size), m_Target(other.m_Target), m_Usage(other.m_Usage), m_IsMapped(false)
	{
		BLT_ASSERT(!other.m_IsMapped, "Cannot move mapped buffer");
		other.m_Id = 0;
	}

	Buffer& Buffer::operator=(Buffer&& other) noexcept
	{
		BLT_ASSERT(!other.m_IsMapped, "Cannot move mapped buffer");
		id_t tempId = m_Id;
		m_Id = other.m_Id;
		m_Size = other.m_Size;
		m_Target = other.m_Target;
		m_Usage = other.m_Usage;
		m_IsMapped = false;
		other.m_Id = tempId;
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

	BufferUsage Buffer::Usage() const
	{
		return m_Usage;
	}

	BufferTarget Buffer::Target() const
	{
		return m_Target;
	}

	id_t Buffer::Id() const
	{
		return m_Id;
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
		BLT_ASSERT(!m_IsMapped, "Cannot Map Mapped buffer");
		Bind();
		void* result = GL_CALL(glMapBuffer((GLenum)m_Target, (GLenum)access));
		m_IsMapped = true;
		return result;
	}

	bool Buffer::Unmap() const
	{
		BLT_ASSERT(m_IsMapped, "Cannot Unmap Unmapped buffer");
		Bind();
		bool result = GL_CALL(glUnmapBuffer((GLenum)m_Target));
		m_IsMapped = false;
		return result;
	}

	void Buffer::Upload(const void* data, size_t size, size_t offset) const
	{
		BLT_ASSERT(size + offset <= Size(), "Could not upload: " + std::to_string(size) + " as buffer is not large enough");
		Bind();
		GL_CALL(glBufferSubData((GLenum)m_Target, (GLintptr)offset, (GLsizeiptr)size, (const GLvoid*)data));
	}

	void Buffer::Download(void* data, size_t size, size_t offset) const
	{
		BLT_ASSERT(size + offset <= Size(), "Could not download: " + std::to_string(size) + " as buffer does not contain that many bytes");
		Bind();
		GL_CALL(glGetBufferSubData((GLenum)m_Target, (GLintptr)offset, (GLsizeiptr)size, (GLvoid*)data));
	}

	void Buffer::Download(void* data) const
	{
		Download(data, Size(), 0);
	}

	void Buffer::Resize(size_t newSize)
	{
		m_Size = newSize;
		GL_CALL(glBufferData((GLenum)m_Target, (GLsizeiptr)Size(), (const GLvoid*)nullptr, (GLenum)Usage()));
	}

	void Buffer::ResizePreserve(size_t newSize)
	{
		size_t oldSize = (newSize < Size()) ? newSize : Size();
		byte* oldData = new byte[oldSize];
		Download(oldData, oldSize, 0);
		Resize(newSize);
		Upload(oldData, oldSize, 0);
		delete[] oldData;
	}

	void Buffer::TestResize(size_t newSize)
	{
		if (newSize >= Size())
		{
			Resize(newSize);
		}
	}

	void Buffer::TestResizePreserve(size_t newSize)
	{
		if (newSize >= Size())
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