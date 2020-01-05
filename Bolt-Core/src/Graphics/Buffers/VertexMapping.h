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

		struct BLT_API AttributeInfo
		{
		public:
			void* BasePtr;
			int Stride;
		};

	private:
		const VertexArray* m_Array;
		std::vector<MappingPtr> m_MappedPtrs;
		mutable std::unordered_map<int, AttributeInfo> m_Attributes;

	public:
		VertexMapping() = default;
		VertexMapping(const std::vector<MappingPtr>& mappedPtrs);
		VertexMapping(const VertexMapping& other) = delete;
		VertexMapping& operator=(const VertexMapping& other) = delete;
		VertexMapping(VertexMapping&& other);
		VertexMapping& operator=(VertexMapping&& other);
		~VertexMapping();

		int VertexCount() const;

		VertexIterator Begin() const;
		VertexIterator End() const;
		VertexIterator GetVertex(int index) const;

		friend class VertexIterator;
		friend class VertexArray;

	private:
		bool HasAttribute(int attribIndex) const;
		std::pair<const MappingPtr&, const MappingAttribute&> GetMappingInfo(int attribIndex) const;
		const AttributeInfo& GetAttribute(int attribIndex) const;
		void* GetAttributePtr(int attribIndex, int vertexIndex) const;
		void SetVertexArray(const VertexArray* vertexArray);

	};

}