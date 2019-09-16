#include "bltpch.h"
#include "ShaderFileReader.h"
#include "Core/Functions.h"

namespace Bolt
{

	ShaderFileReader::ShaderFileReader()
		: m_VertexSource(), m_GeometrySource(), m_FragmentSource(), m_IsDirty(true), m_ShaderInstance(nullptr)
	{

	}

	ShaderFileReader::ShaderFileReader(const Filepath& shaderFile) : ShaderFileReader()
	{
		SetShaderFile(shaderFile);
	}

	ShaderFileReader::ShaderFileReader(const Filepath& vertexFile, const Filepath& fragmentFile) : ShaderFileReader()
	{
		SetVertexFile(vertexFile);
		SetFragmentFile(fragmentFile);
	}

	ShaderFileReader::ShaderFileReader(const Filepath& vertexFile, const Filepath& geometryFile, const Filepath& fragmentFile) : ShaderFileReader()
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

	const blt::string& ShaderFileReader::GetVertexSource() const
	{
		return m_VertexSource;
	}

	const blt::string& ShaderFileReader::GetGeometrySource() const
	{
		return m_GeometrySource;
	}

	const blt::string& ShaderFileReader::GetFragmentSource() const
	{
		return m_FragmentSource;
	}

	void ShaderFileReader::SetVertexFile(const Filepath& file)
	{
		File f = Filesystem::Open(file, OpenMode::Read);
		SetVertexSource(f.ReadText());
	}

	void ShaderFileReader::SetGeometryFile(const Filepath& file)
	{
		File f = Filesystem::Open(file, OpenMode::Read);
		SetGeometrySource(f.ReadText());
	}

	void ShaderFileReader::SetFragmentFile(const Filepath& file)
	{
		File f = Filesystem::Open(file, OpenMode::Read);
		SetFragmentSource(f.ReadText());
	}

	void ShaderFileReader::SetShaderFile(const Filepath& file)
	{
		blt::string vertexSource;
		blt::string geometrySource;
		blt::string fragmentSource;
		File f = Filesystem::Open(file, OpenMode::Read);
		ParseShaderFile(f.ReadText(), vertexSource, geometrySource, fragmentSource);
		SetVertexSource(vertexSource);
		SetGeometrySource(geometrySource);
		SetFragmentSource(fragmentSource);
	}

	void ShaderFileReader::SetVertexSource(const blt::string& source)
	{
		m_VertexSource = source;
		m_IsDirty = true;
	}

	void ShaderFileReader::SetGeometrySource(const blt::string& source)
	{
		m_GeometrySource = source;
		m_IsDirty = true;
	}

	void ShaderFileReader::SetFragmentSource(const blt::string& source)
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
		blt::string vSource = m_VertexSource;
		blt::string gSource = m_GeometrySource;
		blt::string fSource = m_FragmentSource;
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

	void ShaderFileReader::ParseShaderFile(const blt::string& source, blt::string& outVertexSource, blt::string& outGeometrySource, blt::string& outFragmentSource)
	{
		blt::string shaderSources[3];

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != blt::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			BLT_ASSERT(eol != blt::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1;
			blt::string type = source.substr(begin, eol - begin);
			BLT_ASSERT(ShaderTypeFromString(type) != -1, "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == blt::string::npos ? source.size() - 1 : nextLinePos));
		}

		outVertexSource = shaderSources[0];
		outGeometrySource = shaderSources[1];
		outFragmentSource = shaderSources[2];
	}

	int ShaderFileReader::ShaderTypeFromString(const blt::string& str)
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

	std::vector<Bolt::ShaderFileReader::UniformInfo> ShaderFileReader::ProcessShaderSource(blt::string& source)
	{
		std::unordered_map<blt::string, UniformInfo> uniformMap;
		uint32_t uniformIndex = source.find("uniform");
		while (uniformIndex != blt::string::npos)
		{
			uint32_t eol = source.find(';', uniformIndex);
			BLT_ASSERT(eol != blt::string::npos, "Syntax error");
			blt::string line = source.substr(uniformIndex, eol - uniformIndex);
			std::vector<blt::string> parts = line.split(' ');
			BLT_ASSERT(parts.size() >= 3, "Syntax error");
			blt::string& type = parts[1];
			blt::string& name = parts[2];
			bool isArray = false;
			if (name.contains('['))
			{
				name = name.substr(0, name.find('['));
				isArray = true;
			}
			UniformInfo& uniform = uniformMap[name];
			uniform.Dimension = (isArray) ? ValueTypeDim::Array : ValueTypeDim::Single;
			uniform.VariableName = name;
			uniform.Type = GLSLStringToValueType(type);
			uniformIndex = source.find("uniform", eol);
		}

		uint32_t linkIndex = source.find("#link");
		while (linkIndex != blt::string::npos)
		{
			uint32_t start = source.find('[', linkIndex);
			uint32_t eol = source.find(']', start);
			BLT_ASSERT(start != blt::string::npos && eol != blt::string::npos, "Syntax Error");
			blt::string line = source.substr(start + 1, eol - start - 1);
			blt::string type = ReadValue(line, "Type");
			blt::string varname = ReadStringValue(line, "Name");
			blt::string linkName = ReadStringValue(line, "Link");

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

	blt::string ShaderFileReader::ReadValue(const blt::string& line, const blt::string& key)
	{
		uint32_t start = line.find(key);
		BLT_ASSERT(start != blt::string::npos, "No value with key {} exists", key);
		uint32_t equals = line.find('=', start);
		BLT_ASSERT(equals != blt::string::npos, "Invalid syntax");
		uint32_t end = line.find(',', equals);
		if (end == blt::string::npos)
		{
			end = line.find(']', equals);
		}
		BLT_ASSERT(end != blt::string::npos, "Invalid syntax");
		start = line.find_first_not_of(" \t=", equals);
		BLT_ASSERT(start != blt::string::npos, "Invalid syntax");
		return line.substr(start, end - start);
	}

	blt::string ShaderFileReader::ReadStringValue(const blt::string& line, const blt::string& key)
	{
		uint32_t start = line.find(key);
		BLT_ASSERT(start != blt::string::npos, "No value with key {} exists", key);
		uint32_t equals = line.find('=', start);
		BLT_ASSERT(equals != blt::string::npos, "Invalid syntax");
		start = line.find('\"', equals);
		BLT_ASSERT(start != blt::string::npos, "Invalid syntax");
		uint32_t end = line.find('\"', start + 1);
		BLT_ASSERT(end != blt::string::npos, "Invalid syntax");
		return line.substr(start + 1, end - start - 1);
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
				info.Length = 1;
				program.UserUniforms.push_back(info);
			}
		}
	}

}