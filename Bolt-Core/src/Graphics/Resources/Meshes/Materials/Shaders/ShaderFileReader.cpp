#include "bltpch.h"
#include "ShaderFileReader.h"
#include "BoltLib/Functions.h"

namespace Bolt
{

	ShaderFileReader::ShaderFileReader()
		: m_VertexSource(), m_GeometrySource(), m_FragmentSource(), m_IsDirty(true), m_ShaderInstance(nullptr)
	{

	}

	ShaderFileReader::ShaderFileReader(const FilePath& shaderFile) : ShaderFileReader()
	{
		SetShaderFile(shaderFile);
	}

	ShaderFileReader::ShaderFileReader(const FilePath& vertexFile, const FilePath& fragmentFile) : ShaderFileReader()
	{
		SetVertexFile(vertexFile);
		SetFragmentFile(fragmentFile);
	}

	ShaderFileReader::ShaderFileReader(const FilePath& vertexFile, const FilePath& geometryFile, const FilePath& fragmentFile) : ShaderFileReader()
	{
		SetVertexFile(vertexFile);
		SetGeometryFile(geometryFile);
		SetFragmentFile(fragmentFile);
	}

	bool ShaderFileReader::HasVertexSource() const
	{
		return !m_VertexSource.empty();
	}

	bool ShaderFileReader::HasGeometrySource() const
	{
		return !m_GeometrySource.empty();
	}

	bool ShaderFileReader::HasFragmentSource() const
	{
		return !m_FragmentSource.empty();
	}

	const std::string& ShaderFileReader::GetVertexSource() const
	{
		return m_VertexSource;
	}

	const std::string& ShaderFileReader::GetGeometrySource() const
	{
		return m_GeometrySource;
	}

	const std::string& ShaderFileReader::GetFragmentSource() const
	{
		return m_FragmentSource;
	}

	void ShaderFileReader::SetVertexFile(const FilePath& file)
	{
		File f = Filesystem::Open(file, OpenMode::Read);
		SetVertexSource(f.ReadText());
	}

	void ShaderFileReader::SetGeometryFile(const FilePath& file)
	{
		File f = Filesystem::Open(file, OpenMode::Read);
		SetGeometrySource(f.ReadText());
	}

	void ShaderFileReader::SetFragmentFile(const FilePath& file)
	{
		File f = Filesystem::Open(file, OpenMode::Read);
		SetFragmentSource(f.ReadText());
	}

	void ShaderFileReader::SetShaderFile(const FilePath& file)
	{
		std::string vertexSource;
		std::string geometrySource;
		std::string fragmentSource;
		File f = Filesystem::Open(file, OpenMode::Read);
		ParseShaderFile(f.ReadText(), vertexSource, geometrySource, fragmentSource);
		SetVertexSource(vertexSource);
		SetGeometrySource(geometrySource);
		SetFragmentSource(fragmentSource);
	}

	void ShaderFileReader::SetVertexSource(const std::string& source)
	{
		m_VertexSource = source;
		m_IsDirty = true;
	}

	void ShaderFileReader::SetGeometrySource(const std::string& source)
	{
		m_GeometrySource = source;
		m_IsDirty = true;
	}

	void ShaderFileReader::SetFragmentSource(const std::string& source)
	{
		m_FragmentSource = source;
		m_IsDirty = true;
	}

	const Bolt::Ref<Bolt::ShaderInstance>& ShaderFileReader::GetShader() const
	{
		if (m_IsDirty || m_ShaderInstance == nullptr)
		{
			Recompile();
		}
		return m_ShaderInstance;
	}

	Bolt::ShaderLinkContext ShaderFileReader::GetLinkContext() const
	{
		return ShaderLinkContext(GetShader());
	}

	void ShaderFileReader::Recompile() const
	{
		BLT_ASSERT(HasVertexSource() && HasFragmentSource(), "Invalid shader, must have a vertex and fragment shader");
		m_IsDirty = false;
		std::string vSource = m_VertexSource;
		std::string gSource = m_GeometrySource;
		std::string fSource = m_FragmentSource;
		std::vector<UniformInfo> vertexUniforms = ProcessShaderSource(vSource);
		std::vector<UniformInfo> geometryUniforms = ProcessShaderSource(gSource);
		std::vector<UniformInfo> fragmentUniforms = ProcessShaderSource(fSource);

		CompiledShaderProgram vertexShader;
		vertexShader.Source = vSource;
		PopulateShaderUniforms(vertexShader, vertexUniforms);

		CompiledShaderProgram geometryShader;
		geometryShader.Source = gSource;
		PopulateShaderUniforms(geometryShader, geometryUniforms);

		CompiledShaderProgram fragmentShader;
		fragmentShader.Source = fSource;
		PopulateShaderUniforms(fragmentShader, fragmentUniforms);
		
		if (HasGeometrySource())
		{
			m_ShaderInstance = std::make_shared<ShaderInstance>(vertexShader, geometryShader, fragmentShader);
		}
		else
		{
			m_ShaderInstance = std::make_shared<ShaderInstance>(vertexShader, fragmentShader);
		}
	}

	void ShaderFileReader::ParseShaderFile(const std::string& source, std::string& outVertexSource, std::string& outGeometrySource, std::string& outFragmentSource)
	{
		std::string shaderSources[3];

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			BLT_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			BLT_ASSERT(ShaderTypeFromString(type) != -1, "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}

		outVertexSource = shaderSources[0];
		outGeometrySource = shaderSources[1];
		outFragmentSource = shaderSources[2];
	}

	int ShaderFileReader::ShaderTypeFromString(const std::string& str)
	{
		if (str == "vertex")
		{
			return 0;
		}
		if (str == "geometry")
		{
			return 1;
		}
		if (str == "fragment" || str == "pixel")
		{
			return 2;
		}
		return -1;
	}

	std::vector<Bolt::ShaderFileReader::UniformInfo> ShaderFileReader::ProcessShaderSource(std::string& source)
	{
		std::unordered_map<std::string, UniformInfo> uniformMap;
		size_t uniformIndex = source.find("uniform");
		while (uniformIndex != std::string::npos)
		{
			size_t eol = source.find(';', uniformIndex);
			BLT_ASSERT(eol != std::string::npos, "Syntax error");
			std::string line = source.substr(uniformIndex, eol - uniformIndex);
			std::vector<std::string_view> parts = blt::split_view(line, ' ');
			BLT_ASSERT(parts.size() >= 3, "Syntax error");
			std::string_view& type = parts[1];
			std::string_view& name = parts[2];
			bool isArray = false;
			int length = 0;
			if (name.find('[') != std::string::npos)
			{
				size_t start = name.find('[');
				size_t end = name.find(']', start);
				BLT_ASSERT(end != std::string::npos, "Invalid syntax");
				std::string_view lengthString = name.substr(start + 1, end - start - 1);
				name = name.substr(0, start);				
				length = ReadGlslConstInt(std::string(lengthString), source);
				isArray = true;
				BLT_CORE_INFO("ARRAY LENGTH {}", length);
			}
			UniformInfo& uniform = uniformMap[std::string(name)];
			uniform.Dimension = (isArray) ? ValueTypeDim::Array : ValueTypeDim::Single;
			uniform.VariableName = name;
			uniform.Type = GLSLStringToValueType(std::string(type));
			uniform.Length = length;
			uniformIndex = source.find("uniform", eol);
		}

		size_t linkIndex = source.find("#link");
		while (linkIndex != std::string::npos)
		{
			size_t start = source.find('[', linkIndex);
			size_t eol = source.find(']', start);
			BLT_ASSERT(start != std::string::npos && eol != std::string::npos, "Syntax Error");
			std::string line = source.substr(start + 1, eol - start - 1);
			std::string type = ReadValue(line, "Type");
			std::string varname = ReadStringValue(line, "Name");
			std::string linkName = ReadStringValue(line, "Link");

			BLT_ASSERT(uniformMap.find(varname) != uniformMap.end(), "Uniform with name {} does not exist", varname);
			UniformInfo& info = uniformMap[varname];
			info.Meta.IsRenderer = (type == "Renderer");
			if (info.Meta.IsRenderer)
			{
				info.Meta.Uniform = GetRendererUniformFromString(linkName);
				BLT_ASSERT(info.Type == GetTypeOfRendererUniform(info.Meta.Uniform), "Invalid type, {} for renderer uniform {}", type, linkName);
			}
			else
			{
				info.Meta.LinkName = linkName;
			}
			source.erase(linkIndex, eol - linkIndex + 1);
			linkIndex = source.find("#link", linkIndex);
		}
		std::vector<UniformInfo> uniforms;
		uniforms.reserve(uniformMap.size());
		for (auto& pair : uniformMap)
		{
			uniforms.push_back(std::move(pair.second));
		}
		return uniforms;
	}

	std::string ShaderFileReader::ReadValue(const std::string& line, const std::string& key)
	{
		size_t start = line.find(key);
		BLT_ASSERT(start != std::string::npos, "No value with key {} exists", key);
		size_t equals = line.find('=', start);
		BLT_ASSERT(equals != std::string::npos, "Invalid syntax");
		size_t end = line.find(',', equals);
		if (end == std::string::npos)
		{
			end = line.find(']', equals);
		}
		BLT_ASSERT(end != std::string::npos, "Invalid syntax");
		start = line.find_first_not_of(" \t=", equals);
		BLT_ASSERT(start != std::string::npos, "Invalid syntax");
		return line.substr(start, end - start);
	}

	std::string ShaderFileReader::ReadStringValue(const std::string& line, const std::string& key)
	{
		size_t start = line.find(key);
		BLT_ASSERT(start != std::string::npos, "No value with key {} exists", key);
		size_t equals = line.find('=', start);
		BLT_ASSERT(equals != std::string::npos, "Invalid syntax");
		start = line.find('\"', equals);
		BLT_ASSERT(start != std::string::npos, "Invalid syntax");
		size_t end = line.find('\"', start + 1);
		BLT_ASSERT(end != std::string::npos, "Invalid syntax");
		return line.substr(start + 1, end - start - 1);
	}

	int ShaderFileReader::ReadGlslConstInt(const std::string& value, const std::string& source)
	{
		int i = std::atoi(value.c_str());
		if (i > 0)
		{
			return i;
		}
		// value represents a const-int variable
		size_t location = source.find("const int " + value);
		BLT_ASSERT(location != std::string::npos, "Invalid syntax, could not find const int variable {}", value);
		size_t equals = source.find('=', location);
		BLT_ASSERT(equals != std::string::npos, "Invalid syntax");
		size_t end = source.find(';', equals);
		BLT_ASSERT(end != std::string::npos, "Invalid syntax");
		std::string newValue = source.substr(equals + 1, end - equals - 1);
		blt::remove_all(newValue, " \r\t\n");
		return ReadGlslConstInt(newValue, source);
	}

	void ShaderFileReader::PopulateShaderUniforms(CompiledShaderProgram& program, const std::vector<UniformInfo>& uniforms)
	{
		for (const auto& uniform : uniforms)
		{
			if (uniform.Meta.IsRenderer)
			{
				RendererUniformInfo info;
				info.VarName = uniform.VariableName;
				info.Uniform = uniform.Meta.Uniform;
				if (GetTypeDimOfRendererUniform(info.Uniform) == ValueTypeDim::Array)
				{
					info.Length = GetArrayLengthOfRendererUniform(info.Uniform);
				}
				program.RendererUniforms.push_back(info);
			}
			else
			{
				UserUniformInfo info;
				info.VarName = uniform.VariableName;
				info.Type = uniform.Type;
				info.Dimension = uniform.Dimension;
				info.LinkName = uniform.Meta.LinkName;
				info.Length = uniform.Length;
				program.UserUniforms.push_back(info);
			}
		}
	}

}