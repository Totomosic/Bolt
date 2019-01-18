#include "Types.h"

#include "XMLnode.h"

namespace Bolt
{

	XMLnode::XMLnode()
		: Name(""), Attributes(), Data(""), Children(), Parent(nullptr)
	{
	
	}

	XMLnode::XMLnode(const XMLnode& other)
		: Name(other.Name), Attributes(other.Attributes), Data(other.Data), Children(other.Children), Parent(other.Parent)
	{
		for (XMLnode& child : Children)
		{
			child.Parent = this;
		}
	}

	XMLnode& XMLnode::operator=(const XMLnode& other)
	{
		Name = other.Name;
		Attributes = other.Attributes;
		Data = other.Data;
		Children = other.Children;
		Parent = other.Parent;
		for (XMLnode& child : Children)
		{
			child.Parent = this;
		}
		return *this;
	}

	XMLnode::XMLnode(XMLnode&& other)
		: Name(std::move(other.Name)), Attributes(std::move(other.Attributes)), Data(std::move(other.Data)), Children(std::move(other.Children)), Parent(other.Parent)
	{
		for (XMLnode& child : Children)
		{
			child.Parent = this;
		}
	}

	XMLnode& XMLnode::operator=(XMLnode&& other)
	{
		Name = std::move(other.Name);
		Attributes = std::move(other.Attributes);
		Data = std::move(other.Data);
		Children = std::move(other.Children);
		Parent = other.Parent;
		for (XMLnode& child : Children)
		{
			child.Parent = this;
		}
		return *this;
	}

	XMLnode::~XMLnode()
	{

	}

	bool XMLnode::HasAttribute(const blt::string& attributeName) const
	{
		return Attributes.find(attributeName) != Attributes.end();
	}

	bool XMLnode::HasParent() const
	{
		return Parent != nullptr;
	}

	int XMLnode::ChildCount() const
	{
		return Children.size();
	}

	bool XMLnode::HasChildren() const
	{
		return ChildCount() != 0;
	}

	bool XMLnode::HasChild(const blt::string& name) const
	{
		for (const XMLnode& child : Children)
		{
			if (child.Name == name)
			{
				return true;
			}
		}
		return false;
	}

	int XMLnode::IndexOf(const XMLnode& child) const
	{
		for (int i = 0; i < ChildCount(); i++)
		{
			if (&child == &Children.at(i))
			{
				return i;
			}
		}
		return -1;
	}

	int XMLnode::IndexOfName(const XMLnode& child) const
	{
		auto nodes = GetChildren(child.Name);
		for (int i = 0; i < nodes.size(); i++)
		{
			if (nodes.at(i) == &child)
			{
				return i;
			}
		}
		return -1;
	}

	XMLNodePath XMLnode::GetPath() const
	{
		blt::string pathString = "";
		if (Parent != nullptr)
		{
			int index = Parent->IndexOfName(*this);
			blt::string name = Name;
			if (index != 0)
			{
				name += '{' + std::to_string(index) + '}';
			}
			pathString += name;
			const XMLnode* currentNode = Parent;
			while (currentNode->Parent != nullptr)
			{
				int index = currentNode->Parent->IndexOfName(*currentNode);
				blt::string name = currentNode->Name;
				if (index != 0)
				{
					name += '{' + std::to_string(index) + '}';
				}
				pathString.insert(0, name + XMLNodePath::NODE_DELIMETER);
				currentNode = currentNode->Parent;
			}
		}
		XMLNodePath path = XMLNodePath(pathString);
		return path;
	}

	const XMLnode& XMLnode::GetChild(const blt::string& name) const
	{
		for (const XMLnode& child : Children)
		{
			if (child.Name == name)
			{
				return child;
			}
		}
		BLT_ASSERT(false, "Could not find child with name " + name);
		return *(const XMLnode*)nullptr;
	}

	XMLnode& XMLnode::GetChild(const blt::string& name)
	{
		for (XMLnode& child : Children)
		{
			if (child.Name == name)
			{
				return child;
			}
		}
		BLT_ASSERT(false, "Could not find child with name " + name);
		return *(XMLnode*)nullptr;
	}

	const XMLnode& XMLnode::GetChildFromPath(const XMLNodePath& path) const
	{
		path.CachePath(this);
		const XMLnode* currentNode = this;
		for (int index : path.ChildIndices())
		{
			currentNode = &currentNode->Children.at(index);
		}
		return *currentNode;
	}

	XMLnode& XMLnode::GetChildFromPath(const XMLNodePath& path)
	{
		path.CachePath(this);
		XMLnode* currentNode = this;
		for (int index : path.ChildIndices())
		{
			currentNode = &currentNode->Children.at(index);
		}
		return *currentNode;
	}

	std::vector<const XMLnode*> XMLnode::GetChildren(const blt::string& name) const
	{
		std::vector<const XMLnode*> result;
		for (const XMLnode& child : Children)
		{
			if (child.Name == name)
			{
				result.push_back(&child);
			}
		}
		return result;
	}

	std::vector<XMLnode*> XMLnode::GetChildren(const blt::string& name)
	{
		std::vector<XMLnode*> result;
		for (XMLnode& child : Children)
		{
			if (child.Name == name)
			{
				result.push_back(&child);
			}
		}
		return result;
	}

	const XMLnode& XMLnode::SeekRoot() const
	{
		const XMLnode* currentNode = this;
		while (currentNode->Parent != nullptr)
		{
			currentNode = currentNode->Parent;
		}
		return *currentNode;
	}

	XMLnode& XMLnode::SeekRoot()
	{
		XMLnode* currentNode = this;
		while (currentNode->Parent != nullptr)
		{
			currentNode = currentNode->Parent;
		}
		return *currentNode;
	}

	XMLnode& XMLnode::AddChild(const blt::string& name, const std::unordered_map<blt::string, blt::string>& attributes)
	{
		id_t index = Children.size();
		Children.push_back(XMLnode());
		XMLnode& child = Children.at(index);
		child.Name = name;
		child.Attributes = attributes;
		child.Parent = this;
		return child;
	}

	void XMLnode::AddData(const blt::string& data)
	{
		Data += data;
	}

	void XMLnode::AddData(const void* data, uint length)
	{
		blt::string s((const char*)data, length);
		AddData(s);
	}

}