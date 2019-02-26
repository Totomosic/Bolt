#include "Types.h"
#include "ShaderLinkContext.h"

namespace Bolt
{

	ShaderLinkContext::ShaderLinkContext(const ShaderFactory& factory)
		: m_Shader(factory.CreateInstance()), m_Links(), m_UserUniformLinks()
	{
		const std::vector<UserUniformLocation>& uniforms = m_Shader.GetUserUniforms();
		for (int i = 0; i < uniforms.size(); i++)
		{
			const UserUniformLocation& uniform = uniforms[i];
			m_UserUniformLinks[uniform.LinkName] = { false, (id_t)i, 0 };
		}
	}

	const ShaderInstance& ShaderLinkContext::GetShaderInstance() const
	{
		return m_Shader;
	}

	id_t ShaderLinkContext::GetLinkId(const blt::string& linkName) const
	{
		BLT_ASSERT(m_UserUniformLinks.find(linkName) != m_UserUniformLinks.end(), "No link with name {0} exists", linkName);
		const LinkId& id = m_UserUniformLinks.at(linkName);
		BLT_ASSERT(id.IsLinked, "Link has not been linked to a value yet");
		return id.LinkIndex;
	}

	const UniformLinkContainer& ShaderLinkContext::GetLink(const blt::string& linkName) const
	{		
		return GetLink(GetLinkId(linkName));
	}

	UniformLinkContainer& ShaderLinkContext::GetLink(const blt::string& linkName)
	{
		return GetLink(GetLinkId(linkName));
	}
	
	const UniformLinkContainer& ShaderLinkContext::GetLink(id_t linkId) const
	{
		return *m_Links.at(linkId);
	}

	UniformLinkContainer& ShaderLinkContext::GetLink(id_t linkId)
	{
		return *m_Links.at(linkId);
	}

	bool ShaderLinkContext::IsLinked(const blt::string& linkName) const
	{
		BLT_ASSERT(m_UserUniformLinks.find(linkName) != m_UserUniformLinks.end(), "No link with name {0} exists", linkName);
		return m_UserUniformLinks.at(linkName).IsLinked;
	}

	void ShaderLinkContext::ApplyLinks() const
	{
		BLT_ASSERT(std::find_if(m_UserUniformLinks.begin(), m_UserUniformLinks.end(), [](const auto& pair)
		{
			return pair.second.IsLinked == false;
		}) == m_UserUniformLinks.end(), "Not all User uniforms were linked");
		for (const auto& uniform : m_Links)
		{
			uniform->UploadValue(m_Shader.GetShader());
		}
	}

	const UserUniformLocation& ShaderLinkContext::GetUniformLocation(const blt::string& linkName) const
	{
		return m_Shader.GetUserUniforms().at(m_UserUniformLinks.at(linkName).UniformIndex);
	}

	UniformLinkContainer& ShaderLinkContext::AddLink(const blt::string& linkName, std::unique_ptr<UniformLinkContainer>&& linkValue)
	{
		UniformLinkContainer* ptr = linkValue.get();
		id_t linkIndex = m_Links.size();
		m_Links.push_back(std::move(linkValue));
		LinkId& id = m_UserUniformLinks.at(linkName);
		id.IsLinked = true;
		id.LinkIndex = linkIndex;
		return *ptr;
	}

}