#include "FileType.h"

namespace Bolt::Assets
{

	std::unordered_map<std::string, FileType> s_ExtensionFileTypeMap = {
		{ ".png", FileType::PNG },
		{ ".jpeg", FileType::JPEG },
		{ ".jpg", FileType::JPG },
		{ ".obj", FileType::OBJ },
		{ ".shader", FileType::SHADER },
		{ ".glsl", FileType::GLSL },
	};

}