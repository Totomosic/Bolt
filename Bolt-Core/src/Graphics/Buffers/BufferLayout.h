#pragma once
#include "bltpch.h"
#include "BoltLib/Math/Maths.h"
#include "BoltLib/Color.h"

namespace Bolt
{

	BLT_API enum class DataType
	{
		UInt = GL_UNSIGNED_INT,
		Int = GL_INT,
		UByte = GL_UNSIGNED_BYTE,
		Float = GL_FLOAT
	};

	BLT_API inline int GetSizeofDatatype(DataType type)
	{
		switch (type)
		{
		case DataType::UInt:
			return sizeof(GLuint);
		case DataType::Int:
			return sizeof(GLint);
		case DataType::UByte:
			return sizeof(GLubyte);
		case DataType::Float:
			return sizeof(GLfloat);
		}
		BLT_ASSERT(false, "Invalid datatype");
		return 0;
	}

	class BLT_API BufferLayout
	{
	public:
		struct BLT_API VertexAttribute
		{
		public:
			int Index;
			int Count;
			DataType Type;
			bool Normalised;
			int Offset;
		};

		struct BLT_API DefaultLayoutStructure
		{
		public:
			static constexpr int Position = 0;
			static constexpr int Normal = 1;
			static constexpr int TexCoord = 2;
			static constexpr int Color = 3;
			static constexpr int Tangent = 4;
		};

		static constexpr DefaultLayoutStructure DefaultIndices = {};

	private:
		std::vector<VertexAttribute> m_Attributes;
		uint32_t m_Stride;

	public:
		BufferLayout();

		uint32_t Stride() const;
		uint32_t Size() const;
		int OffsetOf(int index) const;
		int AttributeCount() const;
		bool HasAttribute(int index) const;
		const VertexAttribute& GetAttribute(int index) const;
		const std::vector<VertexAttribute>& GetAttributes() const;

		void AddAttribute(int count, DataType type = DataType::Float, bool normalized = false);

		bool operator==(const BufferLayout& other) const;
		bool operator!=(const BufferLayout& other) const;

		template<typename T>
		void AddAttribute(int count, bool normalized = false)
		{
			BLT_ASSERT(false, "Invalid Type");
		}

		friend class VertexArray;

	public:
		static BufferLayout Default();

	private:
		void AddAttribute(const VertexAttribute& attribute);

	};

	template<>
	inline void BufferLayout::AddAttribute<int>(int count, bool normalized)
	{
		AddAttribute(count, DataType::Int, normalized);
	}

	template<>
	inline void BufferLayout::AddAttribute<byte>(int count, bool normalized)
	{
		AddAttribute(count, DataType::UByte, normalized);
	}

	template<>
	inline void BufferLayout::AddAttribute<unsigned int>(int count, bool normalized)
	{
		AddAttribute(count, DataType::UInt, normalized);
	}

	template<>
	inline void BufferLayout::AddAttribute<float>(int count, bool normalized)
	{
		AddAttribute(count, DataType::Float, normalized);
	}

	template<>
	inline void BufferLayout::AddAttribute<Vector2f>(int count, bool normalized)
	{
		AddAttribute<float>(2 * count, normalized);
	}

	template<>
	inline void BufferLayout::AddAttribute<Vector3f>(int count, bool normalized)
	{
		AddAttribute<float>(3 * count, normalized);
	}

	template<>
	inline void BufferLayout::AddAttribute<Vector4f>(int count, bool normalized)
	{
		AddAttribute<float>(4 * count, normalized);
	}

	template<>
	inline void BufferLayout::AddAttribute<Color>(int count, bool normalized)
	{
		AddAttribute<Vector4f>(count, normalized);
	}

}