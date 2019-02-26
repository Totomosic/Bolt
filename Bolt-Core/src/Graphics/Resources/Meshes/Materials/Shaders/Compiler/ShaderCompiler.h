#pragma once
#include "Bolt-Core.h"
#include "ComShaderProgram.h"

namespace Bolt
{
	
	class BLT_API ShaderCompiler
	{
	private:
		static std::unordered_map<blt::string, blt::string> s_Headers;

	public:
		ShaderCompiler() = delete;

		static ComShaderProgram Compile(blt::string sources[3], const Directorypath& rootDirectory = "");

		friend class Shader;

	private:
		static void AddHeader(const blt::string& headerName, const blt::string& headerSource);

		static blt::string PreprocessShaderSource(const blt::string& source, const Directorypath& rootDirectory = "");

	};

}