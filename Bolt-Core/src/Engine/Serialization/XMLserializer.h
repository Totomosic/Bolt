#pragma once
#include "XMLReadTraits.h"
#include "XMLWriteTraits.h"

namespace Bolt
{

	class BLT_API XMLserializer
	{
	public:
		static const std::string POINTER_VALUES_CHILD;

	private:
		XMLnode* m_OutputNode;
		XMLnode* m_InputNode;
		std::unordered_map<void*, id_t> m_SerializedValues;
		id_t m_SerializedCount;

	public:
		XMLserializer();
		XMLserializer(XMLnode& xmlNode, bool writing);

		bool IsWriting() const;

		XMLnode* OutputNode() const;
		XMLnode* InputNode() const;
		void SetOutputNode(XMLnode& xmlNode);
		void SetInputNode(XMLnode& xmlNode);

		template<typename T>
		void Read(T& object)
		{
			BLT_ASSERT(!IsWriting(), "Must have a valid input node set to read");
			object.Transfer(*this, false);
		}

		template<typename T>
		void Write(const std::string& name, T& object)
		{
			BLT_ASSERT(IsWriting(), "Must have a valid output file set to write");
			id_t id = AddSerializedValue((void*)&object);
			XMLnode& pointerValues = OutputNode()->AddChild(POINTER_VALUES_CHILD);
			XMLnode& child = OutputNode()->AddChild(name, { { "id", std::to_string(id) } });
			SetOutputNode(child);
			object.Transfer(*this, true);
			SetOutputNode(*child.Parent);
			ClearSerializedValues();
		}

		// Should only called from an Object's Transfer<>() function
		template<typename T>
		void Transfer(const std::string& name, T* value)
		{
			if (IsWriting())
			{
				XMLWriteTraits<T, XMLserializer>::Transfer(name, value, *OutputNode(), *this);
			}
			else
			{
				XMLReadTraits<T, XMLserializer>::Transfer(name, value, *InputNode(), *this);
			}
		}

		template<typename T>
		void TransferArray(const std::string& name, T* value, uint32_t length)
		{
			if (IsWriting())
			{
				XMLWriteTraits<T, XMLserializer>::TransferArray(name, value, length, *OutputNode(), *this);
			}
		}

		template<typename, typename> friend struct XMLWriteTraits;
		template<typename, typename> friend struct XMLReadTraits;

	private:
		id_t AddSerializedValue(void* value);
		bool HasSerializedValue(void* value) const;
		id_t IdOfSerializedValue(void* value) const;
		void ClearSerializedValues();

	};

}