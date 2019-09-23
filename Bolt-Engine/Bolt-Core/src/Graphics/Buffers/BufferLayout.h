#pragma once
#include "bltpch.h"
#include "Core/Math/Maths.h"
#include "Core/Color.h"

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

		static constexpr int POSITION_INDEX = 0;
		static constexpr int NORMAL_INDEX = 1;
		static constexpr int TEXCOORD_INDEX = 2;
		static constexpr int COLOR_INDEX = 3;
		static constexpr int TANGENT_INDEX = 4;

	private:
		std::unordered_map<int, VertexAttribute> m_Attributes;
		uint32_t m_Stride;

	public:
		BufferLayout();

		uint32_t Stride() const;
		uint32_t Size() const;
		int OffsetOf(int index) const;
		int AttributeCount() const;
		bool HasAttribute(int index) const;
		const VertexAttribute& GetAttribute(int index) const;
		std::vector<VertexAttribute> GetAttributes() const;

		void AddAttribute(int count, DataType type = DataType::Float, bool normalized = false);
		void AddAttribute(int index, int count, DataType type = DataType::Float, bool normalized = false);

		int GetPostionIndex() const;
		int GetNormalIndex() const;
		int GetTexCoordIndex() const;
		int GetColorIndex() const;
		int GetTangentIndex() const;

		bool operator==(const BufferLayout& other) const;
		bool operator!=(const BufferLayout& other) const;

		template<typename T>
		void AddAttribute(int count, bool normalized = false)
		{
			BLT_ASSERT(false, "Unrecognised type T");
		}

		template<>
		void AddAttribute<int>(int count, bool normalized)
		{
			AddAttribute(count, DataType::Int, normalized);
		}

		template<>
		void AddAttribute<byte>(int count, bool normalized)
		{
			AddAttribute(count, DataType::UByte, normalized);
		}

		template<>
		void AddAttribute<unsigned int>(int count, bool normalized)
		{
			AddAttribute(count, DataType::UInt, normalized);
		}

		template<>
		void AddAttribute<float>(int count, bool normalized)
		{
			AddAttribute(count, DataType::Float, normalized);
		}

		template<>
		void AddAttribute<Vector2f>(int count, bool normalized)
		{
			AddAttribute<float>(2 * count, normalized);
		}

		template<>
		void AddAttribute<Vector3f>(int count, bool normalized)
		{
			AddAttribute<float>(3 * count, normalized);
		}

		template<>
		void AddAttribute<Vector4f>(int count, bool normalized)
		{
			AddAttribute<float>(4 * count, normalized);
		}

		template<>
		void AddAttribute<Color>(int count, bool normalized)
		{
			AddAttribute<Vector4f>(count, normalized);
		}

		template<typename T>
		void AddAttribute(int index, int count, bool normalized = false)
		{
			BLT_ASSERT(false, "Unrecognised type T");
		}

		template<>
		void AddAttribute<int>(int index, int count, bool normalized)
		{
			AddAttribute(index, count, DataType::Int, normalized);
		}

		template<>
		void AddAttribute<byte>(int index, int count, bool normalized)
		{
			AddAttribute(index, count, DataType::UByte, normalized);
		}

		template<>
		void AddAttribute<unsigned int>(int index, int count, bool normalized)
		{
			AddAttribute(index, count, DataType::UInt, normalized);
		}

		template<>
		void AddAttribute<float>(int index, int count, bool normalized)
		{
			AddAttribute(index, count, DataType::Float, normalized);
		}

		template<>
		void AddAttribute<Vector2f>(int index, int count, bool normalized)
		{
			AddAttribute<float>(index, 2 * count, normalized);
		}

		template<>
		void AddAttribute<Vector3f>(int index, int count, bool normalized)
		{
			AddAttribute<float>(index, 3 * count, normalized);
		}

		template<>
		void AddAttribute<Vector4f>(int index, int count, bool normalized)
		{
			AddAttribute<float>(index, 4 * count, normalized);
		}

		template<>
		void AddAttribute<Color>(int index, int count, bool normalized)
		{
			AddAttribute<Vector4f>(index, count, normalized);
		}

		friend class VertexArray;

	private:
		void AddAttribute(const VertexAttribute& attribute);
		const std::unordered_map<int, VertexAttribute>& GetAttributesMap() const;

	public:
		static BufferLayout Default();

	};

}