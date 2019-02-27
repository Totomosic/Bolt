#include "Types.h"
#include "MaterialModules.h"

namespace Bolt
{

	MaterialModules::MaterialModules()
		: m_Modules(), m_ModuleTypeMap(), m_Factory()
	{
	
	}

	const std::vector<std::unique_ptr<MaterialModule>>& MaterialModules::Modules() const
	{
		return m_Modules;
	}

	ShaderLinkContext MaterialModules::Compile()
	{
		ShaderLinkContext c = ShaderLinkContext(m_Factory);
		return std::move(c);
	}

	MaterialModule* MaterialModules::AddModule(size_t typeHash, std::unique_ptr<MaterialModule>&& module)
	{
		if (HasModule(typeHash))
		{
			BLT_WARN("MaterialModules already contained module of type {}, only the latest is retrievable with GetModule<>()");
		}
		MaterialModule* ptr = module.get();
		id_t index = m_Modules.size();
		m_Modules.push_back(std::move(module));
		m_ModuleTypeMap[typeHash] = index;
		ptr->AddInstructions(m_Factory);
		return ptr;
	}

	bool MaterialModules::HasModule(size_t typeHash) const
	{
		return m_ModuleTypeMap.find(typeHash) != m_ModuleTypeMap.end();
	}

	MaterialModule* MaterialModules::GetModule(size_t typeHash) const
	{
		return m_Modules[m_ModuleTypeMap.at(typeHash)].get();
	}

}