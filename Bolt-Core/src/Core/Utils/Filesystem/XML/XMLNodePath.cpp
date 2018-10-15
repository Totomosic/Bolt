#include "XMLNodePath.h"
#include "XMLnode.h"

namespace Bolt
{

	XMLNodePath::XMLNodePath()
		: m_IsValid(false)
	{

	}

	XMLNodePath::XMLNodePath(const blt::string& path)
		: m_Path(path), m_IsValid(false)
	{

	}

	XMLNodePath::XMLNodePath(const char* path) : XMLNodePath(blt::string(path))
	{
	
	}

	const blt::string& XMLNodePath::AsString() const
	{
		return m_Path;
	}

	const std::vector<int>& XMLNodePath::ChildIndices() const
	{
		return m_ChildrenIndices;
	}

	void XMLNodePath::SetPath(const blt::string& path)
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
			m_Path.pop_front();
		}
		if (m_Path.back() == NODE_DELIMETER)
		{
			m_Path.pop_back();
		}
		if (m_Path.contains(NODE_DELIMETER))
		{
			std::vector<blt::string> nodeNames = m_Path.split(NODE_DELIMETER);
			const XMLnode* currentNode = node;
			for (blt::string node : nodeNames)
			{
				int index = 0;
				if (node.contains('{') && node.contains('}'))
				{
					uint leftBrace = node.find_first_of('{');
					uint rightBrace = node.find_first_of('}');
					blt::string indexString = node.substr(leftBrace + 1, rightBrace - leftBrace - 1);
					node = node.substr(0, leftBrace);
				}
				const XMLnode* childNode = currentNode->GetChildren(node).at(index);
				m_ChildrenIndices.push_back(currentNode->IndexOf(*childNode));
				currentNode = childNode;
			}
		}
	}

}