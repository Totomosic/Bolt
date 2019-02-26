#pragma once
#include "ShaderFactory.h"
#include "UniformLink.h"

namespace Bolt
{

	class BLT_API ShaderLinkContext
	{
	public:
		struct BLT_API LinkId
		{
		public:
			bool IsLinked = false;
			id_t UniformIndex = 0;
			id_t LinkIndex = 0;
		};

	private:
		ShaderInstance m_Shader;
		std::vector<std::unique_ptr<UniformLinkContainer>> m_Links;
		std::unordered_map<blt::string, LinkId> m_UserUniformLinks;

	public:
		ShaderLinkContext(const ShaderFactory& factory);

		const ShaderInstance& GetShaderInstance() const;
		id_t GetLinkId(const blt::string& linkName) const;
		const UniformLinkContainer& GetLink(const blt::string& linkName) const;
		UniformLinkContainer& GetLink(const blt::string& linkName);
		const UniformLinkContainer& GetLink(id_t linkId) const;
		UniformLinkContainer& GetLink(id_t linkId);
		bool IsLinked(const blt::string& linkName) const;

		void ApplyLinks() const;

		template<typename T>
		const UniformLink<T>& GetLink(const blt::string& linkName) const
		{
			return GetLink<T>(GetLinkId(linkName));
		}

		template<typename T>
		UniformLink<T>& GetLink(const blt::string& linkName)
		{
			return GetLink<T>(GetLinkId(linkName));
		}

		template<typename T>
		const UniformLink<T>& GetLink(id_t linkId) const
		{
			const UserUniformLocation& uniform = GetUniformLocation(linkName);
			BLT_ASSERT(ValidateUniformType<T>(uniform), "Uniform with LinkId {0} does not have type {1} (Type = {2})", linkId, typeid(T).name(), FunctionDef::GetTypeString(uniform.Type));
			return (const UniformLink<T>&)GetLink(linkId);
		}

		template<typename T>
		UniformLink<T>& GetLink(id_t linkId)
		{
			const UserUniformLocation& uniform = GetUniformLocation(linkName);
			BLT_ASSERT(ValidateUniformType<T>(uniform), "Uniform with LinkId {0} does not have type {1} (Type = {2})", linkId, typeid(T).name(), FunctionDef::GetTypeString(uniform.Type));
			return (UniformLink<T>&)GetLink(linkId);
		}

		template<typename T>
		UniformLink<T>& Link(const blt::string& linkName, const T& value)
		{
			BLT_ASSERT(false, "Unsupported uniform type " + typeid(T).name());
			return UniformLink<T>();
		}

		template<>
		UniformLink<int>& Link(const blt::string& linkName, const int& value)
		{
			const UserUniformLocation& uniform = GetUniformLocation(linkName);
			BLT_ASSERT(ValidateUniformType<int>(uniform), "Uniform with LinkName {0} does not have type {1} (Type = {2})", linkName, typeid(int).name(), FunctionDef::GetTypeString(uniform.Type));
			return (UniformLink<int>&)AddLink(linkName, std::make_unique<UniformLink<int>>(m_Shader.GetShader().Id(), GetUniformLocation(linkName).Location, value));
		}

		template<>
		UniformLink<float>& Link(const blt::string& linkName, const float& value)
		{
			const UserUniformLocation& uniform = GetUniformLocation(linkName);
			BLT_ASSERT(ValidateUniformType<float>(uniform), "Uniform with LinkName {0} does not have type {1} (Type = {2})", linkName, typeid(float).name(), FunctionDef::GetTypeString(uniform.Type));
			return (UniformLink<float>&)AddLink(linkName, std::make_unique<UniformLink<float>>(m_Shader.GetShader().Id(), GetUniformLocation(linkName).Location, value));
		}

		template<>
		UniformLink<Vector2i>& Link(const blt::string& linkName, const Vector2i& value)
		{
			const UserUniformLocation& uniform = GetUniformLocation(linkName);
			BLT_ASSERT(ValidateUniformType<Vector2i>(uniform), "Uniform with LinkName {0} does not have type {1} (Type = {2})", linkName, typeid(Vector2i).name(), FunctionDef::GetTypeString(uniform.Type));
			return (UniformLink<Vector2i>&)AddLink(linkName, std::make_unique<UniformLink<Vector2i>>(m_Shader.GetShader().Id(), GetUniformLocation(linkName).Location, value));
		}

		template<>
		UniformLink<Vector3i>& Link(const blt::string& linkName, const Vector3i& value)
		{
			const UserUniformLocation& uniform = GetUniformLocation(linkName);
			BLT_ASSERT(ValidateUniformType<Vector3i>(uniform), "Uniform with LinkName {0} does not have type {1} (Type = {2})", linkName, typeid(Vector3i).name(), FunctionDef::GetTypeString(uniform.Type));
			return (UniformLink<Vector3i>&)AddLink(linkName, std::make_unique<UniformLink<Vector3i>>(m_Shader.GetShader().Id(), GetUniformLocation(linkName).Location, value));
		}

		template<>
		UniformLink<Vector4i>& Link(const blt::string& linkName, const Vector4i& value)
		{
			const UserUniformLocation& uniform = GetUniformLocation(linkName);
			BLT_ASSERT(ValidateUniformType<Vector4i>(uniform), "Uniform with LinkName {0} does not have type {1} (Type = {2})", linkName, typeid(Vector4i).name(), FunctionDef::GetTypeString(uniform.Type));
			return (UniformLink<Vector4i>&)AddLink(linkName, std::make_unique<UniformLink<Vector4i>>(m_Shader.GetShader().Id(), GetUniformLocation(linkName).Location, value));
		}

		template<>
		UniformLink<Vector2f>& Link(const blt::string& linkName, const Vector2f& value)
		{
			const UserUniformLocation& uniform = GetUniformLocation(linkName);
			BLT_ASSERT(ValidateUniformType<Vector2f>(uniform), "Uniform with LinkName {0} does not have type {1} (Type = {2})", linkName, typeid(Vector2f).name(), FunctionDef::GetTypeString(uniform.Type));
			return (UniformLink<Vector2f>&)AddLink(linkName, std::make_unique<UniformLink<Vector2f>>(m_Shader.GetShader().Id(), GetUniformLocation(linkName).Location, value));
		}

		template<>
		UniformLink<Vector3f>& Link(const blt::string& linkName, const Vector3f& value)
		{
			const UserUniformLocation& uniform = GetUniformLocation(linkName);
			BLT_ASSERT(ValidateUniformType<Vector3f>(uniform), "Uniform with LinkName {0} does not have type {1} (Type = {2})", linkName, typeid(Vector3f).name(), FunctionDef::GetTypeString(uniform.Type));
			return (UniformLink<Vector3f>&)AddLink(linkName, std::make_unique<UniformLink<Vector3f>>(m_Shader.GetShader().Id(), GetUniformLocation(linkName).Location, value));
		}

		template<>
		UniformLink<Vector4f>& Link(const blt::string& linkName, const Vector4f& value)
		{
			const UserUniformLocation& uniform = GetUniformLocation(linkName);
			BLT_ASSERT(ValidateUniformType<Vector4f>(uniform), "Uniform with LinkName {0} does not have type {1} (Type = {2})", linkName, typeid(Vector4f).name(), FunctionDef::GetTypeString(uniform.Type));
			return (UniformLink<Vector4f>&)AddLink(linkName, std::make_unique<UniformLink<Vector4f>>(m_Shader.GetShader().Id(), GetUniformLocation(linkName).Location, value));
		}

		template<>
		UniformLink<Color>& Link(const blt::string& linkName, const Color& value)
		{
			const UserUniformLocation& uniform = GetUniformLocation(linkName);
			BLT_ASSERT(ValidateUniformType<Color>(uniform), "Uniform with LinkName {0} does not have type {1} (Type = {2})", linkName, typeid(Color).name(), FunctionDef::GetTypeString(uniform.Type));
			return (UniformLink<Color>&)AddLink(linkName, std::make_unique<UniformLink<Color>>(m_Shader.GetShader().Id(), GetUniformLocation(linkName).Location, value));
		}

		template<>
		UniformLink<Matrix2f>& Link(const blt::string& linkName, const Matrix2f& value)
		{
			const UserUniformLocation& uniform = GetUniformLocation(linkName);
			BLT_ASSERT(ValidateUniformType<Matrix2f>(uniform), "Uniform with LinkName {0} does not have type {1} (Type = {2})", linkName, typeid(Matrix2f).name(), FunctionDef::GetTypeString(uniform.Type));
			return (UniformLink<Matrix2f>&)AddLink(linkName, std::make_unique<UniformLink<Matrix2f>>(m_Shader.GetShader().Id(), GetUniformLocation(linkName).Location, value));
		}

		template<>
		UniformLink<Matrix3f>& Link(const blt::string& linkName, const Matrix3f& value)
		{
			const UserUniformLocation& uniform = GetUniformLocation(linkName);
			BLT_ASSERT(ValidateUniformType<Matrix3f>(uniform), "Uniform with LinkName {0} does not have type {1} (Type = {2})", linkName, typeid(Matrix3f).name(), FunctionDef::GetTypeString(uniform.Type));
			return (UniformLink<Matrix3f>&)AddLink(linkName, std::make_unique<UniformLink<Matrix3f>>(m_Shader.GetShader().Id(), GetUniformLocation(linkName).Location, value));
		}

		template<>
		UniformLink<Matrix4f>& Link(const blt::string& linkName, const Matrix4f& value)
		{
			const UserUniformLocation& uniform = GetUniformLocation(linkName);
			BLT_ASSERT(ValidateUniformType<Matrix4f>(uniform), "Uniform with LinkName {0} does not have type {1} (Type = {2})", linkName, typeid(Matrix4f).name(), FunctionDef::GetTypeString(uniform.Type));
			return (UniformLink<Matrix4f>&)AddLink(linkName, std::make_unique<UniformLink<Matrix4f>>(m_Shader.GetShader().Id(), GetUniformLocation(linkName).Location, value));
		}

		template<>
		UniformLink<ResourcePtr<const Texture2D>>& Link(const blt::string& linkName, const ResourcePtr<const Texture2D>& value)
		{
			const UserUniformLocation& uniform = GetUniformLocation(linkName);
			BLT_ASSERT(ValidateUniformType<ResourcePtr<const Texture2D>>(uniform), "Uniform with LinkName {0} does not have type {1} (Type = {2})", linkName, typeid(ResourcePtr<const Texture2D>).name(), FunctionDef::GetTypeString(uniform.Type));
			return (UniformLink<ResourcePtr<const Texture2D>>&)AddLink(linkName, std::make_unique<UniformLink<ResourcePtr<const Texture2D>>>(m_Shader.GetShader().Id(), GetUniformLocation(linkName).Location, value));
		}

		template<>
		UniformLink<ResourcePtr<Texture2D>>& Link(const blt::string& linkName, const ResourcePtr<Texture2D>& value)
		{
			const UserUniformLocation& uniform = GetUniformLocation(linkName);
			BLT_ASSERT(ValidateUniformType<ResourcePtr<Texture2D>>(uniform), "Uniform with LinkName {0} does not have type {1} (Type = {2})", linkName, typeid(ResourcePtr<Texture2D>).name(), FunctionDef::GetTypeString(uniform.Type));
			return (UniformLink<ResourcePtr<Texture2D>>&)AddLink(linkName, std::make_unique<UniformLink<ResourcePtr<Texture2D>>>(m_Shader.GetShader().Id(), GetUniformLocation(linkName).Location, value));
		}

	private:
		const UserUniformLocation& GetUniformLocation(const blt::string& linkName) const;
		UniformLinkContainer& AddLink(const blt::string& linkName, std::unique_ptr<UniformLinkContainer>&& link);

		template<typename T>
		bool ValidateUniformType(const UserUniformLocation& uniform) const
		{
			BLT_ASSERT(false, "Unsupported uniform type {}", typeid(T).name());
			return false;
		}

		template<>
		bool ValidateUniformType<int>(const UserUniformLocation& uniform) const
		{
			return uniform.Type == ValueType::Int;
		}

		template<>
		bool ValidateUniformType<float>(const UserUniformLocation& uniform) const
		{
			return uniform.Type == ValueType::Float;
		}

		template<>
		bool ValidateUniformType<Vector2i>(const UserUniformLocation& uniform) const
		{
			return uniform.Type == ValueType::Vector2i;
		}

		template<>
		bool ValidateUniformType<Vector3i>(const UserUniformLocation& uniform) const
		{
			return uniform.Type == ValueType::Vector3i;
		}

		template<>
		bool ValidateUniformType<Vector4i>(const UserUniformLocation& uniform) const
		{
			return uniform.Type == ValueType::Vector4i;
		}

		template<>
		bool ValidateUniformType<Vector2f>(const UserUniformLocation& uniform) const
		{
			return uniform.Type == ValueType::Vector2f;
		}

		template<>
		bool ValidateUniformType<Vector3f>(const UserUniformLocation& uniform) const
		{
			return uniform.Type == ValueType::Vector3f;
		}

		template<>
		bool ValidateUniformType<Vector4f>(const UserUniformLocation& uniform) const
		{
			return uniform.Type == ValueType::Vector4f;
		}

		template<>
		bool ValidateUniformType<Color>(const UserUniformLocation& uniform) const
		{
			return uniform.Type == ValueType::Vector4f;
		}

		template<>
		bool ValidateUniformType<Matrix2f>(const UserUniformLocation& uniform) const
		{
			return uniform.Type == ValueType::Matrix2f;
		}

		template<>
		bool ValidateUniformType<Matrix3f>(const UserUniformLocation& uniform) const
		{
			return uniform.Type == ValueType::Matrix3f;
		}

		template<>
		bool ValidateUniformType<Matrix4f>(const UserUniformLocation& uniform) const
		{
			return uniform.Type == ValueType::Matrix4f;
		}

		template<>
		bool ValidateUniformType<ResourcePtr<const Texture2D>>(const UserUniformLocation& uniform) const
		{
			return uniform.Type == ValueType::Texture2D;
		}

		template<>
		bool ValidateUniformType<ResourcePtr<Texture2D>>(const UserUniformLocation& uniform) const
		{
			return uniform.Type == ValueType::Texture2D;
		}

	};

}