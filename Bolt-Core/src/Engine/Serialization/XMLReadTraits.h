#pragma once
#include "Core/Utils/Filesystem/XML/XMLnode.h"

namespace Bolt
{

	template<typename T, typename Backend>
	struct BLT_API XMLReadTraits
	{
	public:
		static void Transfer(const std::string& name, T* value, XMLnode& xmlNode, Backend& backend)
		{
			BLT_ASSERT(xmlNode.HasChild(name), "Unable to find node with name " + name);
			XMLnode& child = xmlNode.GetChild(name);
			backend.SetInputNode(child);
			value->Transfer(backend, false);
			backend.SetInputNode(xmlNode);
		}
	};

	template<typename Backend>
	struct BLT_API XMLReadTraits<int, Backend>
	{
	public:
		static void Transfer(const std::string& name, int* value, XMLnode& xmlNode, Backend& backend)
		{
			BLT_ASSERT(xmlNode.HasChild(name), "Unable to find node with name " + name);
			std::string data = xmlNode.GetChild(name).Data;
			*value = std::stoi(data.c_str());
		}
	};

	template<typename Backend>
	struct BLT_API XMLReadTraits<unsigned int, Backend>
	{
	public:
		static void Transfer(const std::string& name, unsigned int* value, XMLnode& xmlNode, Backend& backend)
		{
			BLT_ASSERT(xmlNode.HasChild(name), "Unable to find node with name " + name);
			std::string data = xmlNode.GetChild(name).Data;
			*value = std::stoi(data.c_str());
		}
	};

	template<typename Backend>
	struct BLT_API XMLReadTraits<char, Backend>
	{
	public:
		static void Transfer(const std::string& name, char* value, XMLnode& xmlNode, Backend& backend)
		{
			BLT_ASSERT(xmlNode.HasChild(name), "Unable to find node with name " + name);
			std::string data = xmlNode.GetChild(name).Data;
			*value = std::stoi(data.c_str());
		}
	};

	template<typename Backend>
	struct BLT_API XMLReadTraits<byte, Backend>
	{
	public:
		static void Transfer(const std::string& name, byte* value, XMLnode& xmlNode, Backend& backend)
		{
			BLT_ASSERT(xmlNode.HasChild(name), "Unable to find node with name " + name);
			std::string data = xmlNode.GetChild(name).Data;
			*value = std::stoi(data.c_str());
		}
	};

	template<typename Backend>
	struct BLT_API XMLReadTraits<int64_t, Backend>
	{
	public:
		static void Transfer(const std::string& name, int64_t* value, XMLnode& xmlNode, Backend& backend)
		{
			BLT_ASSERT(xmlNode.HasChild(name), "Unable to find node with name " + name);
			std::string data = xmlNode.GetChild(name).Data;
			*value = std::stoi(data.c_str());
		}
	};

	template<typename Backend>
	struct BLT_API XMLReadTraits<float, Backend>
	{
	public:
		static void Transfer(const std::string& name, float* value, XMLnode& xmlNode, Backend& backend)
		{
			BLT_ASSERT(xmlNode.HasChild(name), "Unable to find node with name " + name);
			std::string data = xmlNode.GetChild(name).Data;
			*value = std::stof(data.c_str());
		}
	};

	template<typename Backend>
	struct BLT_API XMLReadTraits<double, Backend>
	{
	public:
		static void Transfer(const std::string& name, double* value, XMLnode& xmlNode, Backend& backend)
		{
			BLT_ASSERT(xmlNode.HasChild(name), "Unable to find node with name " + name);
			std::string data = xmlNode.GetChild(name).Data;
			*value = std::stod(data.c_str());
		}
	};

	template<typename T, typename Backend>
	struct BLT_API XMLReadTraits<std::vector<T>, Backend>
	{
	public:
		static void Transfer(const std::string& name, std::vector<T>* value, XMLnode& xmlNode, Backend& backend)
		{
			BLT_ASSERT(xmlNode.HasChild(name), "Unable to find node with name " + name);
			value->clear();
			XMLnode& vectorNode = xmlNode.GetChild(name);
			int length = vectorNode.ChildCount();
			if (length > 0)
			{
				value->resize(length); // Test?
				for (XMLnode& child : vectorNode.Children)
				{
					int index = std::stoi(child.Name.c_str());
					XMLReadTraits<T, Backend>::Transfer(child.Name, &(*value).at(index), vectorNode, backend);
					backend.SetInputNode(xmlNode);
				}
			}
		}
	};

	template<typename TKey, typename TVal, typename Backend>
	struct BLT_API XMLReadTraits<std::unordered_map<TKey, TVal>, Backend>
	{
	public:
		static void Transfer(const std::string& name, std::unordered_map<TKey, TVal>* value, XMLnode& xmlNode, Backend& backend)
		{
			
		}
	};

	template<typename TVal, typename Backend>
	struct BLT_API XMLReadTraits<std::unordered_map<std::string, TVal>, Backend>
	{
	public:
		static void Transfer(const std::string& name, std::unordered_map<std::string, TVal>* value, XMLnode& xmlNode, Backend& backend)
		{
			BLT_ASSERT(xmlNode.HasChild(name), "Unable to find node with name " + name);
			value->clear();
			XMLnode& mapNode = xmlNode.GetChild(name);
			int length = mapNode.ChildCount();
			if (length > 0)
			{
				for (XMLnode& child : mapNode.Children)
				{
					XMLReadTraits<TVal, Backend>::Transfer(child.Name, &(*value)[child.Name], mapNode, backend);
					backend.SetInputNode(xmlNode);
				}
			}
		}
	};

	template<typename T, typename Backend>
	struct BLT_API XMLReadTraits<T*, Backend>
	{
	public:
		static void Transfer(const std::string& name, T** value, XMLnode& xmlNode, Backend& backend)
		{
			
		}
	};

}