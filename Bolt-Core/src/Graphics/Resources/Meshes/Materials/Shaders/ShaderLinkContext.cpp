#include "bltpch.h"
#include "ShaderLinkContext.h"

namespace Bolt
{

	ShaderLinkContext::ShaderLinkContext(const std::shared_ptr<ShaderInstance>& shaderInstance)
		: m_Shader(shaderInstance), m_Links(), m_UserUniformLinks()
	{
		const std::vector<UserUniformLocation>& uniforms = m_Shader->GetUserUniforms();
		for (int i = 0; i < uniforms.size(); i++)
		{
			const UserUniformLocation& uniform = uniforms[i];
			m_UserUniformLinks[uniform.LinkName] = { false, (id_t)i, 0, uniform.RequiresLink };
			if (uniform.DefaultValue != nullptr)
			{
				AddLink(uniform.LinkName, uniform.DefaultValue->CreateLinkContainer(m_Shader->GetShader().Id(), uniform.Location));
			}
		}
	}

	ShaderLinkContext::ShaderLinkContext(const ShaderLinkContext& other)
		: m_Shader(other.m_Shader), m_Links(), m_UserUniformLinks(other.m_UserUniformLinks)
	{
		m_Links.reserve(other.m_Links.size());
		other.CopyLinksTo(*this);
	}

	ShaderLinkContext& ShaderLinkContext::operator=(const ShaderLinkContext& other)
	{
		m_Shader = other.m_Shader;
		m_Links.reserve(other.m_Links.size());
		m_UserUniformLinks = other.m_UserUniformLinks;
		other.CopyLinksTo(*this);
		return *this;
	}

	const ShaderInstance& ShaderLinkContext::GetShaderInstance() const
	{
		return *m_Shader;
	}

	id_t ShaderLinkContext::GetLinkId(const blt::string& linkName, int index) const
	{
		if (index >= 0)
		{
			return GetLinkId(linkName + '[' + std::to_string(index) + ']');
		}
		BLT_ASSERT(m_UserUniformLinks.find(linkName) != m_UserUniformLinks.end(), "No link with name {0} exists", linkName);
		const LinkId& id = m_UserUniformLinks.at(linkName);
		BLT_ASSERT(id.IsLinked, "Link has not been linked to a value yet");
		return id.LinkIndex;
	}

	const UniformLinkContainer& ShaderLinkContext::GetLink(const blt::string& linkName, int index) const
	{		
		return GetLink(GetLinkId(linkName, index));
	}

	UniformLinkContainer& ShaderLinkContext::GetLink(const blt::string& linkName, int index)
	{
		return GetLink(GetLinkId(linkName, index));
	}
	
	const UniformLinkContainer& ShaderLinkContext::GetLink(id_t linkId) const
	{
		return *m_Links.at(linkId);
	}

	UniformLinkContainer& ShaderLinkContext::GetLink(id_t linkId)
	{
		return *m_Links.at(linkId);
	}

	bool ShaderLinkContext::IsLinked(const blt::string& linkName, int index) const
	{
		if (index >= 0)
		{
			return IsLinked(linkName + '[' + std::to_string(index) + ']');
		}
		BLT_ASSERT(m_UserUniformLinks.find(linkName) != m_UserUniformLinks.end(), "No link with name {0} exists", linkName);
		return m_UserUniformLinks.at(linkName).IsLinked;
	}

	bool ShaderLinkContext::HasLink(const blt::string& linkName, int index) const
	{
		if (index >= 0)
		{
			return HasLink(linkName + '[' + std::to_string(index) + ']');
		}
		return m_UserUniformLinks.find(linkName) != m_UserUniformLinks.end();
	}

	void ShaderLinkContext::ApplyLinks() const
	{
		BLT_ASSERT(std::find_if(m_UserUniformLinks.begin(), m_UserUniformLinks.end(), [](const auto& pair)
		{
			if (!pair.second.IsLinked && pair.second.RequiresLink)
			{
				BLT_CORE_ERROR(pair.first + " is not linked");
			}
			return pair.second.IsLinked == false && pair.second.RequiresLink;
		}) == m_UserUniformLinks.end(), "Not all User uniforms were linked");
		int index = 0;
		m_Shader->GetShader().Bind();
		for (const auto& uniform : m_Links)
		{
			uniform->UploadValue(m_Shader->GetShader(), index);
			index++;
		}
	}

	const UserUniformLocation& ShaderLinkContext::GetUniformLocation(const blt::string& linkName) const
	{
		BLT_ASSERT(m_UserUniformLinks.find(linkName) != m_UserUniformLinks.end(), "No link exists with name {}", linkName);
		return m_Shader->GetUserUniforms().at(m_UserUniformLinks.at(linkName).UniformIndex);
	}

	UniformLinkContainer& ShaderLinkContext::AddLink(const blt::string& linkName, std::unique_ptr<UniformLinkContainer>&& linkValue)
	{
		UniformLinkContainer* ptr = linkValue.get();
		LinkId& id = m_UserUniformLinks.at(linkName);
		if (id.IsLinked)
		{
			// This link already exists
			m_Links[id.LinkIndex] = std::move(linkValue);
			return *ptr;
		}
		id.IsLinked = true;
		id_t linkIndex = m_Links.size();
		id.LinkIndex = linkIndex;
		m_Links.push_back(std::move(linkValue));		
		return *ptr;
	}

	void ShaderLinkContext::CopyLinksTo(ShaderLinkContext& other) const
	{
		other.m_Links.clear();
		for (auto& pair : other.m_UserUniformLinks)
		{
			pair.second.IsLinked = false;
		}
		for (const auto& linkPair : m_UserUniformLinks)
		{
			if (linkPair.second.IsLinked)
			{
				other.AddLink(linkPair.first, m_Links.at(linkPair.second.LinkIndex)->Clone(other.GetShaderInstance().GetShader().Id(), other.GetUniformLocation(linkPair.first).Location));
			}
		}
	}

}