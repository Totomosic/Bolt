#pragma once
#include "Uniform.h"

namespace Bolt
{

	enum class UniformUploadType
	{
		Recurring,
		Once
	};

	class BLT_API UniformManager
	{
	public:
		struct BLT_API UniformInfo
		{
		public:
			std::unique_ptr<Uniform> UniformValue;
			UniformUploadType Type;

		public:
			UniformInfo();
			UniformInfo(const UniformInfo& other);
			UniformInfo& operator=(const UniformInfo& other);
			UniformInfo(UniformInfo&& other);
			UniformInfo& operator=(UniformInfo&& other);

		};

	private:
		mutable std::unordered_map<blt::string, UniformInfo> m_Uniforms;

	public:
		UniformManager();

		bool HasUniform(const blt::string& uniformName) const;
		Uniform* AddUniform(std::unique_ptr<Uniform>&& uniform, UniformUploadType type);

		template<typename T>
		T& AddUniform(const blt::string& uniformName, const T& value, UniformUploadType type)
		{
			std::unique_ptr<UniformInstance<T>> uniform = std::make_unique<UniformInstance<T>>();
			uniform->UniformName = uniformName;
			uniform->Value = value;
			UniformInstance<T>* ptr = (UniformInstance<T>*)AddUniform(std::move(uniform), type);
			return ptr->Value;
		}

		template<typename T>
		T& GetUniform(const blt::string& uniformName)
		{
			return ((UniformInstance<T>*)m_Uniforms[uniformName].UniformValue.get())->Value;
		}

		void RemoveUniform(const blt::string& uniformName) const;
		void UploadAll(const Shader* shader) const;

	};

}