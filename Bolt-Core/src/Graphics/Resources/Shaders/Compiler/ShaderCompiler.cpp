#include "ShaderCompiler.h"

namespace Bolt
{

	std::unordered_map<blt::string, blt::string> ShaderCompiler::s_Headers = std::unordered_map<blt::string, blt::string>();

	ComShaderProgram ShaderCompiler::Compile(blt::string sources[3], const Directorypath& rootDirectory)
	{
		ComShaderProgram program;
		program.Sources[0] = PreprocessShaderSource(sources[0], rootDirectory);
		program.Sources[1] = PreprocessShaderSource(sources[1], rootDirectory);
		program.Sources[2] = PreprocessShaderSource(sources[2], rootDirectory);
		return program;
	}

	void ShaderCompiler::AddHeader(const blt::string& headerName, const blt::string& headerSource)
	{
		s_Headers[headerName] = headerSource;
	}

	blt::string ShaderCompiler::PreprocessShaderSource(const blt::string& source, const Directorypath& rootDirectory)
	{
		blt::string result = source;
		size_t index = result.find("#include");
		while (index != blt::string::npos)
		{
			size_t quote0 = result.find_first_of('"', index);
			size_t quote1 = result.find_first_of('"', quote0 + 1);
			blt::string header = result.substr(quote0 + 1, quote1 - quote0 - 1);
			if (s_Headers.find(header) != s_Headers.end())
			{
				blt::string headerSource = s_Headers[header] + '\n';
				result.erase(index, quote1 - index + 1);
				result.insert(index, headerSource);
			}
			index = result.find("#include", quote1);
		}
		return result;
	}

}