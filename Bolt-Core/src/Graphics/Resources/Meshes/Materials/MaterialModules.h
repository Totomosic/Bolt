#pragma once
#include "MaterialModule.h"

namespace Bolt
{

	class BLT_API MaterialModules
	{
	private:
		std::vector<std::unique_ptr<MaterialModule>> m_Modules;
		std::unordered_map<size_t, id_t> m_ModuleTypeMap;
		ShaderFactory m_Factory;

	public:
		MaterialModules();

		const std::vector<std::unique_ptr<MaterialModule>>& Modules() const;

		template<typename T>
		bool HasModule() const
		{
			return HasModule(typeid(T).hash_code());
		}

		template<typename T>
		T& GetModule() const
		{
			return *(T*)GetModule(typeid(T).hash_code());
		}

		template<typename T, typename... Args>
		T& AddModule(Args&&... args)
		{
			return *(T*)AddModule(typeid(T).hash_code(), std::make_unique<T>(std::forward<Args>(std::move(args))...));
		}

		ShaderLinkContext Compile();

	private:
		MaterialModule* AddModule(size_t typeHash, std::unique_ptr<MaterialModule>&& module);
		bool HasModule(size_t typeHash) const;
		MaterialModule* GetModule(size_t typeHash) const;

	};

}