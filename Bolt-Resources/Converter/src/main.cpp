#include "Logging.h"
#include <iostream>
#include <string>

void usage(const char* programname)
{
	std::cout << "Usage: " << programname << " [-i] <input_files> [-o] <output_files>" << std::endl;
	exit(EXIT_FAILURE);
}

int main(int argc, char** argv)
{
	Bolt::Log::Initialize();
	if (argc < 2)
	{
		usage(argv[0]);
	}
	bool isInput = true;
	std::vector<std::string> inputFiles;
	std::vector<std::string> outputFiles;
	for (int i = 1; i < argc; i++)
	{
		std::string str(argv[i]);
		if (str == "-i")
		{
			isInput = true;
		}
		else if (str == "-f")
		{
			isInput = false;
		}
		else
		{
			((isInput) ? inputFiles : outputFiles).push_back(str);
		}
	}
	for (int i = outputFiles.size(); i < inputFiles.size(); i++)
	{
		outputFiles.push_back(inputFiles[i] + ".out");
	}
	return 0;
}