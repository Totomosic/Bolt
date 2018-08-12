#pragma once
#include "..\GLprivate.h"
#include "Iterators\__Iterators__.h"
#include "VertexBuffer.h"
#include "ArrayDescriptor.h"
#include "IteratorManager.h"

namespace Bolt
{

	enum class RenderMode : GLenum
	{
		Triangles = GL_TRIANGLES,
		TriangleStrip = GL_TRIANGLE_STRIP,
		TriangleFan = GL_TRIANGLE_FAN,
		Lines = GL_LINES,
		LineStrip = GL_LINE_STRIP,
		LineLoop = GL_LINE_LOOP,
		Points = GL_POINTS
	};

	class BLT_API VertexArray : GLprivate
	{
	private:
		id_t m_Id;
		std::vector<std::unique_ptr<VertexBuffer>> m_Vertices;
		RenderMode m_RenderMode;
		ArrayDescriptor m_Descriptor;
		IteratorManager m_IteratorManager;

	public:
		VertexArray(RenderMode mode = RenderMode::Triangles);
		VertexArray(VertexArray&& other) noexcept;
		VertexArray& operator=(VertexArray&& other) noexcept;
		~VertexArray() override;
		VertexArray(const VertexArray& other) = delete;
		VertexArray& operator=(const VertexArray& other) = delete;

		const ArrayDescriptor& Descriptor() const;
		const IteratorManager& Iterators() const;
		RenderMode GetRenderMode() const;
		int VertexBufferCount() const;
		int VertexCount() const;
		id_t ID() const;

		void Bind() const;
		void Unbind() const;

		// VertexBuffer must have same VertexCount() as all other VertexBuffers, if this is the first then no constraint
		VertexBuffer& AddVertexBuffer(std::unique_ptr<VertexBuffer>&& buffer);
		VertexBuffer& CreateVertexBuffer(size_t size, BufferLayout layout, BufferUsage usage = BufferUsage::StaticDraw);
		VertexBuffer& CreateVertexBuffer(const void* data, size_t size, BufferLayout layout, BufferUsage usage = BufferUsage::StaticDraw);

		template<typename T>
		AttributeIterator<T> GetIterator(int attributeIndex) const
		{
			BLT_ASSERT(m_Descriptor.HasAttribute(attributeIndex), "Attribute with index " + std::to_string(attributeIndex) + " does not exist");
			VertexBuffer* buffer = m_Descriptor.GetAttribute(attributeIndex);
			m_MappedBuffers++;
			return AttributeIterator<T>(((byte*)m_MappedBuffers.Request(buffer)) + buffer->Layout().OffsetOf(attributeIndex), this, &buffer->Layout(), attributeIndex);
		}

		VertexIterator GetVertex(int index);
		VertexIterator Begin();
		VertexIterator End();

		void SetRenderMode(RenderMode mode);

		template<typename> friend class AttributeIterator;
		friend class AttributeSetter;

	private:
		void Create();

		template<typename T>
		void FreeAttributeIterator(AttributeIterator<T>& iterator) const
		{
			m_MappedBuffers.Free(m_Descriptor.GetAttribute(iterator.AttributeIndex()));
			m_MappedIterators--;
		}

		bool ValidateAttribute(int attribIndex, DataType type, int count) const;
		
	};

}