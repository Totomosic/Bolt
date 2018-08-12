#include "UniformManager.h"

namespace Bolt
{

	UniformManager::UniformInfo::UniformInfo()
		: Type(), UniformValue()
	{
	
	}

	UniformManager::UniformInfo::UniformInfo(const UniformInfo& other)
		: Type(other.Type), UniformValue(other.UniformValue->Clone())
	{
	
	}

	UniformManager::UniformInfo& UniformManager::UniformInfo::operator=(const UniformInfo& other)
	{
		Type = other.Type;
		UniformValue = other.UniformValue->Clone();
		return *this;
	}

	UniformManager::UniformInfo::UniformInfo(UniformInfo&& other)
		: Type(other.Type), UniformValue(std::move(other.UniformValue))
	{

	}

	UniformManager::UniformInfo& UniformManager::UniformInfo::operator=(UniformInfo&& other)
	{
		Type = other.Type;
		UniformValue = std::move(other.UniformValue);
		return *this;
	}

	UniformManager::UniformManager()
		: m_Uniforms()
	{

	}

	bool UniformManager::HasUniform(const blt::string& uniformName) const
	{
		return m_Uniforms.find(uniformName) != m_Uniforms.end();
	}

	Uniform* UniformManager::AddUniform(std::unique_ptr<Uniform>&& uniform, UniformUploadType type)
	{
		UniformInfo& info = m_Uniforms[uniform->UniformName];
		info.Type = type;
		info.UniformValue = std::move(uniform);
		return info.UniformValue.get();
	}

	void UniformManager::RemoveUniform(const blt::string& uniformName) const
	{
		m_Uniforms.erase(uniformName);
	}

	void UniformManager::UploadAll(const Shader* shader) const
	{
		std::vector<const blt::string*> needDelete;
		for (const auto& pair : m_Uniforms)
		{
			pair.second.UniformValue->Upload(shader);
			if (pair.second.Type == UniformUploadType::Once)
			{
				needDelete.push_back(&pair.first);
			}
		}
		for (const blt::string* str : needDelete)
		{
			RemoveUniform(*str);
		}
	}

}