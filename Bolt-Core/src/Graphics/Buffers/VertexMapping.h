#pragma once
#include "bltpch.h"
#include "Iterators/VertexIterator.h"

namespace Bolt
{

	class VertexArray;

	class BLT_API VertexMapping
	{
	public:
		struct BLT_API MappingAttribute
		{
		public:
			int Index;
			int Offset;
			int Size;
		};

		struct BLT_API MappingPtr
		{
		public:
			void* Ptr;
			std::vector<MappingAttribute> Attributes;
			int Stride;
		};

	private:
		const VertexArray* m_Array;
		std::vector<MappingPtr> m_MappedPtrs;
		std::unordered_map<int, int> m_AttributeMap;

	public:
		VertexMapping(const std::vector<MappingPtr>& mappedPtrs);
		VertexMapping(const VertexMapping& other) = delete;
		VertexMapping& operator=(const VertexMapping& other) = delete;
		VertexMapping(VertexMapping&& other);
		VertexMapping& operator=(VertexMapping&& other);
		~VertexMapping();

		VertexIterator Begin() const;
		VertexIterator End() const;
		VertexIterator GetVertex(int index) const;

		friend class VertexIterator;
		friend class VertexArray;

	private:
		bool HasAttribute(int attribIndex) const;
		void* GetAttributePtr(int attribIndex, int vertexIndex) const;
		void SetVertexArray(const VertexArray* vertexArray);

	};

}