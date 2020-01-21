#pragma once
#include "XMLNodePath.h"

namespace Bolt
{

	struct BLT_API XMLnode
	{
	public:
		std::string Name;
		std::unordered_map<std::string, std::string> Attributes;
		std::string Data;
		std::vector<XMLnode> Children;
		XMLnode* Parent;

	public:
		XMLnode();
		XMLnode(const XMLnode& other);
		XMLnode& operator=(const XMLnode& other);
		XMLnode(XMLnode&& other);
		XMLnode& operator=(XMLnode&& other);
		~XMLnode();

		bool HasAttribute(const std::string& attribute) const;
		bool HasParent() const;
		int ChildCount() const;
		bool HasChildren() const;
		int IndexOf(const XMLnode& child) const;
		int IndexOfName(const XMLnode& child) const;
		XMLNodePath GetPath() const;

		bool HasChild(const std::string& name) const;
		const XMLnode& GetChild(const std::string& name) const;
		XMLnode& GetChild(const std::string& name);
		const XMLnode& GetChildFromPath(const XMLNodePath& path) const;
		XMLnode& GetChildFromPath(const XMLNodePath& path);
		std::vector<const XMLnode*> GetChildren(const std::string& name) const;
		std::vector<XMLnode*> GetChildren(const std::string& name);

		const XMLnode& SeekRoot() const;
		XMLnode& SeekRoot();
		XMLnode& AddChild(const std::string& name, const std::unordered_map<std::string, std::string>& attributes = std::unordered_map<std::string, std::string>());
		void AddData(const std::string& data);
		void AddData(const void* data, uint32_t length);

	};

}