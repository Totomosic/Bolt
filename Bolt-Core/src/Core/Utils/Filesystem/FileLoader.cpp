#include "FileLoader.h"

namespace Bolt
{

	XMLnode FileLoader::LoadXML(File& xmlFile)
	{
		blt::string filedata = xmlFile.ReadText();
		filedata.replace_all('\n', "");
		filedata.replace_all('\t', "");
		filedata.replace_all('\r', "");
		XMLnode root;
		XMLnode* current = &root;

		size_t startIndex = filedata.find_first_of('<', 0);
		size_t endIndex = filedata.find_first_of('>', startIndex + 1);

		while (endIndex != blt::string::npos)
		{
			blt::string tagData = filedata.substr(startIndex, endIndex - startIndex);
			std::vector<blt::string> tagParts = tagData.split(' ');
			blt::string& tagName = tagParts[0];
		}

		return root;
	}

}