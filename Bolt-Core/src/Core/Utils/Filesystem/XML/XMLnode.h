#pragma once
#include "XMLNodePath.h"

namespace Bolt
{

	struct BLT_API XMLnode
	{
	public:
		blt::string Name;
		std::unordered_map<blt::string, blt::string> Attributes;
		blt::string Data;
		std::vector<XMLnode> Children;
		XMLnode* Parent;

	public:
		XMLnode();
		XMLnode(const XMLnode& other);
		XMLnode& operator=(const XMLnode& other);
		XMLnode(XMLnode&& other);
		XMLnode& operator=(XMLnode&& other);
		~XMLnode();

		bool HasAttribute(const blt::string& attribute) const;
		bool HasParent() const;
		int ChildCount() const;
		bool HasChildren() const;
		int IndexOf(const XMLnode& child) const;
		int IndexOfName(const XMLnode& child) const;
		XMLNodePath GetPath() const;

		bool HasChild(const blt::string& name) const;
		const XMLnode& GetChild(const blt::string& name) const;
		XMLnode& GetChild(const blt::string& name);
		const XMLnode& GetChildFromPath(const XMLNodePath& path) const;
		XMLnode& GetChildFromPath(const XMLNodePath& path);
		std::vector<const XMLnode*> GetChildren(const blt::string& name) const;
		std::vector<XMLnode*> GetChildren(const blt::string& name);

		const XMLnode& SeekRoot() const;
		XMLnode& SeekRoot();
		XMLnode& AddChild(const blt::string& name, const std::unordered_map<blt::string, blt::string>& attributes = std::unordered_map<blt::string, blt::string>());
		void AddData(const blt::string& data);
		void AddData(const void* data, uint32_t length);

	};

}