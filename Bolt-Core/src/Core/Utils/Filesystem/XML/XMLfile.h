#pragma once
#include "../File.h"
#include "XMLnode.h"

namespace Bolt
{

	struct BLT_API XMLfile : public File
	{
	private:
		mutable std::vector<std::string> m_CurrentTags;
		mutable bool m_AddedData;
		mutable bool m_WasLastEndTag;
		mutable std::string m_CurrentString;

	public:
		XMLfile();
		XMLfile(const FilePath& filename);

		void BeginXML(const std::string& name, const std::unordered_map<std::string, std::string>& attributes = std::unordered_map<std::string, std::string>()) const;
		void EndXML() const;
		void WriteXMLData(const std::string& text) const;
		void WriteXMLData(const void* data, uint32_t length) const;
		void WriteXMLFile() const;

		void WriteXML(const XMLnode& node) const;
		XMLnode LoadXML() const;

	private:
		std::pair<std::string, std::unordered_map<std::string, std::string>> GetNameAndAttributesFromTag(const std::string& tag) const;
		void WriteXMLnode(const XMLnode& node, int tabCount, std::string& currentString) const;
		void AddTabs(int tabCount, std::string& currentString) const;
		void ProcessTags(const std::string& data, XMLnode* parent) const;

	};

}