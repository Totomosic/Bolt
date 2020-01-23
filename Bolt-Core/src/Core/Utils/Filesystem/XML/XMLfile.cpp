#include "bltpch.h"

#include "XMLfile.h"

namespace Bolt
{

	XMLfile::XMLfile() : XMLfile("")
	{
	
	}

	XMLfile::XMLfile(const FilePath& filename) : File(filename),
		m_CurrentTags(), m_AddedData(true), m_WasLastEndTag(false), m_CurrentString()
	{
	
	}

	void XMLfile::BeginXML(const std::string& name, const std::unordered_map<std::string, std::string>& attributes) const
	{
		if (!m_AddedData)
		{
			m_CurrentString += '\n';
		}
		for (int i = 0; i < (int)m_CurrentTags.size(); i++)
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
		std::string currentTag = m_CurrentTags.back();
		m_CurrentTags.pop_back();
		if (m_WasLastEndTag)
		{
			for (int i = 0; i < (int)m_CurrentTags.size(); i++)
			{
				m_CurrentString += '\t';
			}
		}	
		m_CurrentString += "</" + currentTag + ">\n";
		m_AddedData = true;
		m_WasLastEndTag = true;
	}

	void XMLfile::WriteXMLData(const std::string& text) const
	{
		m_CurrentString += text;
		m_AddedData = true;
		m_WasLastEndTag = false;
	}

	void XMLfile::WriteXMLData(const void* data, uint32_t length) const
	{
		std::string text((const char*)data, length);
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
		std::string data;
		WriteXMLnode(node, tabCount, data);
		WriteText(data);
	}
	
	XMLnode XMLfile::LoadXML() const
	{
		std::string data = ReadText();
		XMLnode root;

		ProcessTags(data, &root);
		if (root.ChildCount() == 1)
		{
			return root.Children.at(0);
		}
		return root;
	}

	std::pair<std::string, std::unordered_map<std::string, std::string>> XMLfile::GetNameAndAttributesFromTag(const std::string& tag) const
	{
		std::pair<std::string, std::unordered_map<std::string, std::string>> result;
		size_t firstSpace = tag.find_first_of(' ');
		if (firstSpace == std::string::npos)
		{
			// The tag contains no attributes and only contains the name
			result.first = tag;
		}
		else
		{
			result.first = tag.substr(0, firstSpace);
			while (firstSpace != std::string::npos)
			{
				size_t begin = firstSpace + 1;
				size_t equals = tag.find_first_of('=', begin);
				size_t firstQuote = tag.find_first_of('"', equals);
				size_t secondQuote = tag.find_first_of('"', firstQuote + 1);
				if (equals == std::string::npos || firstQuote == std::string::npos || secondQuote == std::string::npos)
				{
					return { "", {} };
				}
				std::string key = tag.substr(begin, equals - begin);
				std::string value = tag.substr(firstQuote + 1, secondQuote - firstQuote - 1);
				result.second[key] = value;
				firstSpace = tag.find_first_of(' ', secondQuote);
			}
		}		
		return result;
	}

	void XMLfile::WriteXMLnode(const XMLnode& node, int tabCount, std::string& currentString) const
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

	void XMLfile::AddTabs(int tabCount, std::string& currentString) const
	{
		for (int i = 0; i < tabCount; i++)
		{
			currentString += '\t';
		}
	}

	void XMLfile::ProcessTags(const std::string& data, XMLnode* parent) const
	{
		size_t begin = data.find_first_of('<', 0);
		bool hadChild = false;
		while (begin != std::string::npos)
		{
			size_t beginTagEnd = data.find_first_of('>', begin);
			if (beginTagEnd == std::string::npos)
			{
				break;
			}
			std::string beginTagData = data.substr(begin + 1, beginTagEnd - begin - 1);
			auto nameAttributes = GetNameAndAttributesFromTag(beginTagData);
			std::string endTagName = "</" + nameAttributes.first + '>';
			size_t end = data.find(endTagName, beginTagEnd);
			if (end != std::string::npos)
			{
				hadChild = true;
				XMLnode& node = parent->AddChild(nameAttributes.first, nameAttributes.second);
				std::string nodeData = data.substr(beginTagEnd + 1, end - beginTagEnd - 1);
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