#include "Types.h"

#include "XMLfile.h"

namespace Bolt
{

	XMLfile::XMLfile() : XMLfile("")
	{
	
	}

	XMLfile::XMLfile(const Filepath& filename) : File(filename),
		m_CurrentTags(), m_AddedData(true), m_WasLastEndTag(false), m_CurrentString()
	{
	
	}

	void XMLfile::BeginXML(const blt::string& name, const std::unordered_map<blt::string, blt::string>& attributes) const
	{
		if (!m_AddedData)
		{
			m_CurrentString += '\n';
		}
		for (int i = 0; i < m_CurrentTags.size(); i++)
		{
			m_CurrentString += '\t';
		}
		m_CurrentString += "<" + name;
		for (const auto& pair : attributes)
		{
			m_CurrentString += " " + pair.first + "=\"" + pair.second + '"';
		}
		m_CurrentString += ">";
		m_CurrentTags.push_back(name);
		m_AddedData = false;
		m_WasLastEndTag = false;
	}

	void XMLfile::EndXML() const
	{
		blt::string currentTag = m_CurrentTags.back();
		m_CurrentTags.pop_back();
		if (m_WasLastEndTag)
		{
			for (int i = 0; i < m_CurrentTags.size(); i++)
			{
				m_CurrentString += '\t';
			}
		}	
		m_CurrentString += "</" + currentTag + ">\n";
		m_AddedData = true;
		m_WasLastEndTag = true;
	}

	void XMLfile::WriteXMLData(const blt::string& text) const
	{
		m_CurrentString += text;
		m_AddedData = true;
		m_WasLastEndTag = false;
	}

	void XMLfile::WriteXMLData(const void* data, uint length) const
	{
		blt::string text((const char*)data, length);
		WriteXMLData(text);
	}

	void XMLfile::WriteXMLFile() const
	{
		if (m_CurrentString.back() == '\n')
		{
			m_CurrentString.pop_back();
		}
		WriteText(m_CurrentString);
		m_CurrentString.clear();
		m_AddedData = true;
		m_WasLastEndTag = false;
		m_CurrentTags.clear();
	}

	void XMLfile::WriteXML(const XMLnode& node) const
	{
		int tabCount = 0;
		blt::string data;
		WriteXMLnode(node, tabCount, data);
		WriteText(data);
	}
	
	XMLnode XMLfile::LoadXML() const
	{
		blt::string data = ReadText();
		XMLnode root;

		ProcessTags(data, &root);
		if (root.ChildCount() == 1)
		{
			return root.Children.at(0);
		}
		return root;
	}

	std::pair<blt::string, std::unordered_map<blt::string, blt::string>> XMLfile::GetNameAndAttributesFromTag(const blt::string& tag) const
	{
		std::pair<blt::string, std::unordered_map<blt::string, blt::string>> result;
		id_t firstSpace = tag.find_first_of(' ');
		if (firstSpace == blt::string::npos)
		{
			// The tag contains no attributes and only contains the name
			result.first = tag;
		}
		else
		{
			result.first = tag.substr(0, firstSpace);
			while (firstSpace != blt::string::npos)
			{
				id_t begin = firstSpace + 1;
				id_t equals = tag.find_first_of('=', begin);
				id_t firstQuote = tag.find_first_of('"', equals);
				id_t secondQuote = tag.find_first_of('"', firstQuote + 1);
				if (equals == blt::string::npos || firstQuote == blt::string::npos || secondQuote == blt::string::npos)
				{
					return { "", {} };
				}
				blt::string key = tag.substr(begin, equals - begin);
				blt::string value = tag.substr(firstQuote + 1, secondQuote - firstQuote - 1);
				result.second[key] = value;
				firstSpace = tag.find_first_of(' ', secondQuote);
			}
		}		
		return result;
	}

	void XMLfile::WriteXMLnode(const XMLnode& node, int tabCount, blt::string& currentString) const
	{
		AddTabs(tabCount, currentString);
		currentString += '<' + node.Name;
		for (const auto& pair : node.Attributes)
		{
			currentString += ' ' + pair.first + "=\"" + pair.second + '"';
		}
		currentString += '>' + node.Data;
		for (const XMLnode& child : node.Children)
		{
			currentString += '\n';
			WriteXMLnode(child, tabCount + 1, currentString);
		}
		if (node.ChildCount() > 0)
		{
			currentString += '\n';
			AddTabs(tabCount, currentString);
		}
		currentString += "</" + node.Name + '>';
	}

	void XMLfile::AddTabs(int tabCount, blt::string& currentString) const
	{
		for (int i = 0; i < tabCount; i++)
		{
			currentString += '\t';
		}
	}

	void XMLfile::ProcessTags(const blt::string& data, XMLnode* parent) const
	{
		id_t begin = data.find_first_of('<', 0);
		bool hadChild = false;
		while (begin != blt::string::npos)
		{
			id_t beginTagEnd = data.find_first_of('>', begin);
			if (beginTagEnd == blt::string::npos)
			{
				begin = data.find_first_of('<', begin + 1);
				continue;
			}
			blt::string beginTagData = data.substr(begin + 1, beginTagEnd - begin - 1);
			auto nameAttributes = GetNameAndAttributesFromTag(beginTagData);
			blt::string endTagName = "</" + nameAttributes.first + '>';
			id_t end = data.find(endTagName, beginTagEnd);
			if (end != blt::string::npos)
			{
				hadChild = true;
				XMLnode& node = parent->AddChild(nameAttributes.first, nameAttributes.second);
				blt::string nodeData = data.substr(beginTagEnd + 1, end - beginTagEnd - 1);
				ProcessTags(nodeData, &node);
				begin = data.find_first_of('<', end + 1);
			}
			else
			{
				break;
			}
		}
		if (!hadChild)
		{
			parent->Data = data;
		}
	}

}