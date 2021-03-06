#pragma once
#include "Iterators/VertexIterator.h"
#include "VertexBuffer.h"
#include "ArrayDescriptor.h"
#include "Core/Tasks/TaskManager.h"

namespace Bolt
{

	BLT_API enum class RenderMode : GLenum
	{
		Triangles = GL_TRIANGLES,
		TriangleStrip = GL_TRIANGLE_STRIP,
		TriangleFan = GL_TRIANGLE_FAN,
		Lines = GL_LINES,
		LineStrip = GL_LINE_STRIP,
		LineLoop = GL_LINE_LOOP,
		Points = GL_POINTS
	};

	class BLT_API VertexArray
	{
	private:
		id_t m_Id;
		std::vector<std::unique_ptr<VertexBuffer>> m_Vertices;
		RenderMode m_RenderMode;
		ArrayDescriptor m_Descriptor;

	public:
		VertexArray(RenderMode mode = RenderMode::Triangles);
		VertexArray(const VertexArray& other) = delete;
		VertexArray& operator=(const VertexArray& other) = delete;
		VertexArray(VertexArray&& other) noexcept;
		VertexArray& operator=(VertexArray&& other) noexcept;
		~VertexArray();

		const ArrayDescriptor& Descriptor() const;
		RenderMode GetRenderMode() const;
		int VertexBufferCount() const;
		int VertexCount() const;
		id_t Id() const;
		bool IsMapped() const;

		void Bind() const;
		void Unbind() const;

		// VertexBuffer must have same VertexCount() as all other VertexBuffers, if this is the first then no constraint
		VertexBuffer& AddVertexBuffer(std::unique_ptr<VertexBuffer>&& buffer);
		VertexBuffer& CreateVertexBuffer(size_t size, const BufferLayout& layout, BufferUsage usage = BufferUsage::StaticDraw);
		VertexBuffer& CreateVertexBuffer(const void* data, size_t size, const BufferLayout& layout, BufferUsage usage = BufferUsage::StaticDraw);
		VertexBuffer& GetVertexBuffer(int index);
		void SetRenderMode(RenderMode mode);
		std::unique_ptr<VertexArray> Clone() const;

		friend class VertexMapping;

	private:
		void Create();
		bool ValidateAttribute(int attribIndex, DataType type, int count) const;		
	};

}