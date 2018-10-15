#pragma once
#include "Core\Utils\Filesystem\XML\XMLfile.h"

namespace Bolt
{

	template<typename T, typename Backend>
	struct BLT_API XMLWriteTraits
	{
	public:
		static void Transfer(const blt::string& name, T* value, XMLnode& xmlNode, Backend& backend)
		{
			id_t id = backend.AddSerializedValue(value);
			XMLnode& child = xmlNode.AddChild(name, { { "id", std::to_string(id) } });
			backend.SetOutputNode(child);
			value->Transfer(backend, true);			
			backend.SetOutputNode(xmlNode);
		}

		static void TransferArray(const blt::string& name, T* value, uint length, XMLnode& xmlNode, Backend& backend)
		{
			id_t id = backend.AddSerializedValue(value);
			XMLnode& child = xmlNode.AddChild(name, { { "id", std::to_string(id) }, { "type", "array" }, { "size", std::to_string(length) } });
			for (uint i = 0; i < length; i++)
			{
				XMLWriteTraits<T, Backend>::Transfer(std::to_string(i), (value + i), child, backend);
				backend.SetOutputNode(xmlNode);
			}
		}

	};

	template<typename Backend>
	struct BLT_API XMLWriteTraits<int, Backend>
	{
	public:
		static void Transfer(const blt::string& name, int* value, XMLnode& xmlNode, Backend& backend)
		{
			id_t id = backend.AddSerializedValue(value);
			XMLnode& child = xmlNode.AddChild(name, { { "type", "int" }, { "id", std::to_string(id) } });
			child.AddData(std::to_string(*value));
		}
	};

	template<typename Backend>
	struct BLT_API XMLWriteTraits<unsigned int, Backend>
	{
	public:
		static void Transfer(const blt::string& name, unsigned int* value, XMLnode& xmlNode, Backend& backend)
		{
			id_t id = backend.AddSerializedValue(value);
			XMLnode& child = xmlNode.AddChild(name, { { "type", "uint" }, { "id", std::to_string(id) } });
			child.AddData(std::to_string(*value));
		}
	};

	template<typename Backend>
	struct BLT_API XMLWriteTraits<char, Backend>
	{
	public:
		static void Transfer(const blt::string& name, char* value, XMLnode& xmlNode, Backend& backend)
		{
			id_t id = backend.AddSerializedValue(value);
			XMLnode& child = xmlNode.AddChild(name, { { "type", "char" }, { "id", std::to_string(id) } });
			child.AddData(std::to_string(*value));
		}
	};

	template<typename Backend>
	struct BLT_API XMLWriteTraits<byte, Backend>
	{
	public:
		static void Transfer(const blt::string& name, byte* value, XMLnode& xmlNode, Backend& backend)
		{
			id_t id = backend.AddSerializedValue(value);
			XMLnode& child = xmlNode.AddChild(name, { { "type", "byte" }, { "id", std::to_string(id) } });
			child.AddData(std::to_string(*value));
		}
	};

	template<typename Backend>
	struct BLT_API XMLWriteTraits<int64, Backend>
	{
	public:
		static void Transfer(const blt::string& name, int64* value, XMLnode& xmlNode, Backend& backend)
		{
			id_t id = backend.AddSerializedValue(value);
			XMLnode& child = xmlNode.AddChild(name, { { "type", "long" }, { "id", std::to_string(id) } });
			child.AddData(std::to_string(*value));
		}
	};

	template<typename Backend>
	struct BLT_API XMLWriteTraits<float, Backend>
	{
	public:
		static void Transfer(const blt::string& name, float* value, XMLnode& xmlNode, Backend& backend)
		{
			id_t id = backend.AddSerializedValue(value);
			XMLnode& child = xmlNode.AddChild(name, { { "type", "float" }, { "id", std::to_string(id) } });
			child.AddData(std::to_string(*value));
		}
	};

	template<typename Backend>
	struct BLT_API XMLWriteTraits<double, Backend>
	{
	public:
		static void Transfer(const blt::string& name, double* value, XMLnode& xmlNode, Backend& backend)
		{
			id_t id = backend.AddSerializedValue(value);
			XMLnode& child = xmlNode.AddChild(name, { { "type", "double" }, { "id", std::to_string(id) } });
			child.AddData(std::to_string(*value));
		}
	};

	template<typename T, typename Backend>
	struct BLT_API XMLWriteTraits<std::vector<T>, Backend>
	{
	public:
		static void Transfer(const blt::string& name, std::vector<T>* value, XMLnode& xmlNode, Backend& backend)
		{
			id_t id = backend.AddSerializedValue(value);
			XMLnode& child = xmlNode.AddChild(name, { { "type", "vector" }, { "size", std::to_string(value->size()) }, { "id", std::to_string(id) } });
			int index = 0;
			for (T& val : *value)
			{
				XMLWriteTraits<T, Backend>::Transfer(std::to_string(index), &val, child, backend);
				index++;
				backend.SetOutputNode(xmlNode);
			}
		}
	};

	template<typename TKey, typename TVal, typename Backend>
	struct BLT_API XMLWriteTraits<std::unordered_map<TKey, TVal>, Backend>
	{
	public:
		static void Transfer(const blt::string& name, std::unordered_map<TKey, TVal>* value, XMLnode& xmlNode, Backend& backend)
		{
			id_t id = backend.AddSerializedValue(value);
			XMLnode& child = xmlNode.AddChild(name, { { "type", "unordered_map" }, { "size", std::to_string(value->size()) }, { "id", std::to_string(id) } });
			for (auto& pair : *value)
			{
				XMLWriteTraits<TVal, Backend>::Transfer(std::to_string(pair.first), &pair.second, child, backend);
				backend.SetOutputNode(xmlNode);
			}
		}
	};

	template<typename TVal, typename Backend>
	struct BLT_API XMLWriteTraits<std::unordered_map<blt::string, TVal>, Backend>
	{
	public:
		static void Transfer(const blt::string& name, std::unordered_map<blt::string, TVal>* value, XMLnode& xmlNode, Backend& backend)
		{
			id_t id = backend.AddSerializedValue(value);
			XMLnode& child = xmlNode.AddChild(name, { { "type", "unordered_map" }, { "size", std::to_string(value->size()) }, { "id", std::to_string(id) } });
			for (auto& pair : *value)
			{
				XMLWriteTraits<TVal, Backend>::Transfer(pair.first, &pair.second, child, backend);
				backend.SetOutputNode(xmlNode);
			}		
		}
	};

	template<typename T, typename Backend>
	struct BLT_API XMLWriteTraits<T*, Backend>
	{
	public:
		static void Transfer(const blt::string& name, T** value, XMLnode& xmlNode, Backend& backend)
		{
			if (backend.HasSerializedValue((void*)*value))
			{
				id_t id = backend.IdOfSerializedValue((void*)*value);
				id_t ptrId = backend.AddSerializedValue((void*)value);
				XMLnode& child = xmlNode.AddChild(name, { { "type", "pointer" }, { "id", std::to_string(ptrId) } });
				child.AddData(std::to_string(id));
			}
			else
			{
				blt::string id = "null";
				XMLnode* currentNode = &xmlNode;
				if (*value != nullptr)
				{
					XMLWriteTraits<T, Backend>::Transfer(name + "_value", *value, xmlNode.SeekRoot().GetChild(Backend::POINTER_VALUES_CHILD), backend);
					id = std::to_string(backend.IdOfSerializedValue((void*)*value));
					backend.SetOutputNode(xmlNode);
				}
				id_t ptrId = backend.AddSerializedValue(value);
				XMLnode& child = currentNode->AddChild(name, { { "type", "pointer" }, { "id", std::to_string(ptrId) } });
				child.AddData(id);
			}
		}
	};

	template<typename T, typename Backend>
	struct BLT_API XMLWriteTraits<std::unique_ptr<T>, Backend>
	{
	public:
		static void Transfer(const blt::string& name, std::unique_ptr<T>* value, XMLnode& xmlNode, Backend& backend)
		{
			XMLWriteTraits<T, Backend>::Transfer(name, value->get(), xmlNode, backend);
		}
	};

}