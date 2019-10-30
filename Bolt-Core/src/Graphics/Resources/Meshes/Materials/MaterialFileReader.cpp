#include "bltpch.h"
#include "MaterialFileReader.h"

namespace Bolt
{

	MaterialFileReader::MaterialFileReader()
		: m_Reader(), m_IsTransparent(false)
	{

	}

	MaterialFileReader::MaterialFileReader(const Filepath& shaderFile) : MaterialFileReader()
	{
		m_Reader.SetShaderFile(shaderFile);
	}

	MaterialFileReader::MaterialFileReader(const Filepath& vertexFile, const Filepath& fragmentFile) : MaterialFileReader()
	{
		m_Reader.SetVertexFile(vertexFile);
		m_Reader.SetFragmentFile(fragmentFile);
	}

	MaterialFileReader::MaterialFileReader(const Filepath& vertexFile, const Filepath& geometryFile, const Filepath& fragmentFile) : MaterialFileReader()
	{
		m_Reader.SetVertexFile(vertexFile);
		m_Reader.SetGeometryFile(geometryFile);
		m_Reader.SetFragmentFile(fragmentFile);
	}

	const Bolt::ShaderFileReader& MaterialFileReader::GetShaderReader() const
	{
		return m_Reader;
	}

	bool MaterialFileReader::IsTransparent() const
	{
		return m_IsTransparent;
	}

	void MaterialFileReader::SetIsTransparent(bool isTransparent)
	{
		m_IsTransparent = isTransparent;
	}

}