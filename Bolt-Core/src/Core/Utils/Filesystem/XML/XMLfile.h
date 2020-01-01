#pragma once
#include "../File.h"
#include "XMLnode.h"

namespace Bolt
{

	struct BLT_API XMLfile : public File
	{
	private:
		mutable std::vector<blt::string> m_CurrentTags;
		mutable bool m_AddedData;
		mutable bool m_WasLastEndTag;
		mutable blt::string m_CurrentString;

	public:
		XMLfile();
		XMLfile(const FilePath& filename);

		void BeginXML(const blt::string& name, const std::unordered_map<blt::string, blt::string>& attributes = std::unordered_map<blt::string, blt::string>()) const;
		void EndXML() const;
		void WriteXMLData(const blt::string& text) const;
		void WriteXMLData(const void* data, uint32_t length) const;
		void WriteXMLFile() const;

		void WriteXML(const XMLnode& node) const;
		XMLnode LoadXML() const;

	private:
		std::pair<blt::string, std::unordered_map<blt::string, blt::string>> GetNameAndAttributesFromTag(const blt::string& tag) const;
		void WriteXMLnode(const XMLnode& node, int tabCount, blt::string& currentString) const;
		void AddTabs(int tabCount, blt::string& currentString) const;
		void ProcessTags(const blt::string& data, XMLnode* parent) const;

	};

}