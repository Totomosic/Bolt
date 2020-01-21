#include "bltpch.h"

#include "XMLNodePath.h"
#include "XMLnode.h"

namespace Bolt
{

	XMLNodePath::XMLNodePath()
		: m_IsValid(false)
	{

	}

	XMLNodePath::XMLNodePath(const std::string& path)
		: m_Path(path), m_IsValid(false)
	{

	}

	XMLNodePath::XMLNodePath(const char* path) : XMLNodePath(std::string(path))
	{
	
	}

	const std::string& XMLNodePath::AsString() const
	{
		return m_Path;
	}

	const std::vector<int>& XMLNodePath::ChildIndices() const
	{
		return m_ChildrenIndices;
	}

	void XMLNodePath::SetPath(const std::string& path)
	{
		m_Path = path;
		m_IsValid = false;
	}

	void XMLNodePath::CachePath(const XMLnode* node) const
	{
		if (m_IsValid && node == m_CurrentNode)
		{
			return;
		}
		ProcessPathString(node);
	}

	void XMLNodePath::ProcessPathString(const XMLnode* node) const
	{
		m_CurrentNode = node;
		m_ChildrenIndices.clear();
		if (m_Path.front() == NODE_DELIMETER)
		{
			m_Path.erase(m_Path.begin());
		}
		if (m_Path.back() == NODE_DELIMETER)
		{
			m_Path.pop_back();
		}
		if (m_Path.find(NODE_DELIMETER) != std::string::npos)
		{
			std::vector<std::string_view> nodeNames = blt::split_view(m_Path, NODE_DELIMETER);
			const XMLnode* currentNode = node;
			for (std::string_view& node : nodeNames)
			{
				int index = 0;
				if (node.find('{') != std::string::npos && node.find('}') != std::string::npos)
				{
					size_t leftBrace = node.find_first_of('{');
					size_t rightBrace = node.find_first_of('}');
					std::string_view indexString = node.substr(leftBrace + 1, rightBrace - leftBrace - 1);
					node = node.substr(0, leftBrace);
				}
				const XMLnode* childNode = currentNode->GetChildren(std::string(node)).at(index);
				m_ChildrenIndices.push_back(currentNode->IndexOf(*childNode));
				currentNode = childNode;
			}
		}
	}

}