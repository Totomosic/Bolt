#include "VertexArray.h"

namespace Bolt
{

	VertexArray::VertexArray(RenderMode mode) : GLprivate(),
		m_Id(0), m_Vertices(), m_RenderMode(mode), m_Descriptor(), m_IteratorManager(this)
	{
		Create();
	}

	VertexArray::VertexArray(VertexArray&& other) noexcept
		: m_Id(other.m_Id), m_Vertices(std::move(other.m_Vertices)), m_RenderMode(other.m_RenderMode), m_Descriptor(std::move(other.m_Descriptor)), m_IteratorManager(other.m_IteratorManager)
	{
		BLT_ASSERT(!other.m_IteratorManager.IsMapped(), "Unable to move VertexArray while iterators remain outstanding");
		other.m_Id = 0;
	}

	VertexArray& VertexArray::operator=(VertexArray&& other) noexcept
	{
		BLT_ASSERT(!other.m_IteratorManager.IsMapped(), "Unable to move VertexArray while iterators remain outstanding");
		std::vector<std::unique_ptr<VertexBuffer>> tempVector = std::move(m_Vertices);
		id_t tempID = m_Id;
		m_Id = other.m_Id;
		m_RenderMode = other.m_RenderMode;
		m_Vertices = std::move(other.m_Vertices);
		m_Descriptor = other.m_Descriptor;
		m_IteratorManager = other.m_IteratorManager;
		other.m_Id = tempID;
		other.m_Vertices = std::move(tempVector);
		return *this;
	}

	VertexArray::~VertexArray()
	{
		if (m_Id != 0)
		{
			GL_CALL(glDeleteVertexArrays(1, &m_Id));
		}
	}

	const ArrayDescriptor& VertexArray::Descriptor() const
	{
		return m_Descriptor;
	}

	const IteratorManager& VertexArray::Iterators() const
	{
		return m_IteratorManager;
	}

	RenderMode VertexArray::GetRenderMode() const
	{
		return m_RenderMode;
	}

	int VertexArray::VertexBufferCount() const
	{
		return m_Vertices.size();
	}

	int VertexArray::VertexCount() const
	{
		BLT_ASSERT(VertexBufferCount() > 0, "No vertex buffers");
		return (*m_Vertices.begin())->VertexCount();
	}

	id_t VertexArray::ID() const
	{
		return m_Id;
	}

	void VertexArray::Bind() const
	{
		GL_CALL(glBindVertexArray(m_Id));
	}

	void VertexArray::Unbind() const
	{
		GL_CALL(glBindVertexArray(0));
	}

	VertexBuffer& VertexArray::AddVertexBuffer(std::unique_ptr<VertexBuffer>&& buffer)
	{
		if (VertexBufferCount() > 0)
		{
			BLT_ASSERT(buffer->VertexCount() == VertexCount(), "Buffer vertex count mismatch");
		}
		int index = m_Vertices.size();
		m_Vertices.push_back(std::move(buffer));
		VertexBuffer& thisBuffer = *m_Vertices[index];
		const BufferLayout& layout = thisBuffer.Layout();
		Bind();
		thisBuffer.Bind();
		for (const auto& pair : layout.GetAttributesMap())
		{
			GL_CALL(glEnableVertexAttribArray(pair.first));
			if (pair.second.Type == DataType::UInt || pair.second.Type == DataType::Int)
			{
				glVertexAttribIPointer(pair.second.Index, pair.second.Count, (GLenum)pair.second.Type, layout.Stride(), (const GLvoid*)pair.second.Offset);
			}
			else
			{
				glVertexAttribPointer(pair.second.Index, pair.second.Count, (GLenum)pair.second.Type, pair.second.Normalised, layout.Stride(), (const GLvoid*)pair.second.Offset);
			}	
			//glVertexAttribPointer(pair.second.Index, pair.second.Count, (GLenum)pair.second.Type, pair.second.Normalised, layout.Stride(), (const GLvoid*)pair.second.Offset);
		}
		thisBuffer.Unbind();
		m_Descriptor.AddVertexBuffer(&thisBuffer);
		return thisBuffer;
	}

	VertexBuffer& VertexArray::CreateVertexBuffer(size_t size, BufferLayout layout, BufferUsage usage)
	{
		return CreateVertexBuffer(nullptr, size, layout, usage);
	}

	VertexBuffer& VertexArray::CreateVertexBuffer(const void* data, size_t size, BufferLayout layout, BufferUsage usage)
	{
		std::unique_ptr<VertexBuffer> buffer = std::make_unique<VertexBuffer>(data, size, layout, usage);
		return AddVertexBuffer(std::move(buffer));
	}

	VertexIterator VertexArray::GetVertex(int index)
	{
		return m_IteratorManager.CreateNewIterator(index);
	}

	VertexIterator VertexArray::Begin()
	{
		return GetVertex(0);
	}

	VertexIterator VertexArray::End()
	{
		return GetVertex(VertexCount());
	}

	void VertexArray::SetRenderMode(RenderMode mode)
	{
		m_RenderMode = mode;
	}

	std::unique_ptr<VertexArray> VertexArray::Clone() const
	{
		std::unique_ptr<VertexArray> vertexArray = std::make_unique<VertexArray>(GetRenderMode());
		for (const std::unique_ptr<VertexBuffer>& buffer : m_Vertices)
		{
			vertexArray->AddVertexBuffer(buffer->Clone());
		}
		return std::move(vertexArray);
	}

	void VertexArray::Create()
	{
		GL_CALL(glGenVertexArrays(1, &m_Id));
	}

	bool VertexArray::ValidateAttribute(int attribIndex, DataType type, int count) const
	{
		const BufferLayout::VertexAttribute& attrib =  m_Descriptor.GetAttribute(attribIndex)->Layout().GetAttribute(attribIndex);
		return attrib.Type == type && attrib.Count == count;
	}

}