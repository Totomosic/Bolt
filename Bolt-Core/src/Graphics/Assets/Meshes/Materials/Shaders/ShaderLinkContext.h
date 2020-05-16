#pragma once
#include "ShaderInstance.h"
#include "UniformLink.h"

#include "BoltLib/Math/Maths.h"
#include "Graphics/Assets/Textures/Texture2D.h"
#include "Graphics/Assets/Textures/Fonts/Font.h"

namespace Bolt
{

	namespace Detail
	{

		template<typename T>
		inline bool ValidateUniformType(const UserUniformLocation& uniform)
		{
			BLT_ASSERT(false, "Unsupported uniform type {}", typeid(T).name());
			return false;
		}

		template<>
		inline bool ValidateUniformType<int>(const UserUniformLocation& uniform)
		{
			return uniform.Type == ValueType::Int;
		}

		template<>
		inline bool ValidateUniformType<float>(const UserUniformLocation& uniform)
		{
			return uniform.Type == ValueType::Float;
		}

		template<>
		inline bool ValidateUniformType<Vector2f>(const UserUniformLocation& uniform)
		{
			return uniform.Type == ValueType::Vector2f;
		}

		template<>
		inline bool ValidateUniformType<Vector3f>(const UserUniformLocation& uniform)
		{
			return uniform.Type == ValueType::Vector3f;
		}

		template<>
		inline bool ValidateUniformType<Vector4f>(const UserUniformLocation& uniform)
		{
			return uniform.Type == ValueType::Vector4f;
		}

		template<>
		inline bool ValidateUniformType<Color>(const UserUniformLocation& uniform)
		{
			return uniform.Type == ValueType::Vector4f;
		}

		template<>
		inline bool ValidateUniformType<Matrix2f>(const UserUniformLocation& uniform)
		{
			return uniform.Type == ValueType::Matrix2f;
		}

		template<>
		inline bool ValidateUniformType<Matrix3f>(const UserUniformLocation& uniform)
		{
			return uniform.Type == ValueType::Matrix3f;
		}

		template<>
		inline bool ValidateUniformType<Matrix4f>(const UserUniformLocation& uniform)
		{
			return uniform.Type == ValueType::Matrix4f;
		}

		template<>
		inline bool ValidateUniformType<AssetHandle<Texture2D>>(const UserUniformLocation& uniform)
		{
			return uniform.Type == ValueType::Texture2D;
		}

	}

	// Represents a shader and the uniform values associated with it.
	// Makes up the main part of Materials
	class BLT_API ShaderLinkContext
	{
	public:
		struct BLT_API LinkId
		{
		public:
			bool IsLinked = false;
			bool RequiresLink = true;
			id_t UniformIndex = 0;
			id_t LinkIndex = 0;
		};

	private:
		Ref<ShaderInstance> m_Shader;
		std::vector<std::unique_ptr<UniformLinkContainer>> m_Links;
		std::unordered_map<std::string, LinkId> m_UserUniformLinks;

	public:
		ShaderLinkContext(const Ref<ShaderInstance>& shaderInstance);
		ShaderLinkContext(const ShaderLinkContext& other);
		ShaderLinkContext& operator=(const ShaderLinkContext& other);
		ShaderLinkContext(ShaderLinkContext&& other) = default;
		ShaderLinkContext& operator=(ShaderLinkContext&& other) = default;
		~ShaderLinkContext() = default;

		std::vector<std::string> GetLinkNames() const;

		const ShaderInstance& GetShaderInstance() const;
		// Return a unique identifier for the link represented by linkName (use index when linking array)
		id_t GetLinkId(const std::string& linkName, int index = -1) const;
		// Returns the container with the value for the link with linkName (use index when referring to array)
		const UniformLinkContainer& GetLink(const std::string& linkName, int index = -1) const;
		// Returns the container with the value for the link with linkName (use index when referring to array)
		UniformLinkContainer& GetLink(const std::string& linkName, int index = -1);
		// Returns the container with the value for the link with id linkId
		const UniformLinkContainer& GetLink(id_t linkId) const;
		// Returns the container with the value for the link with id linkId
		UniformLinkContainer& GetLink(id_t linkId);
		// Returns whether the link with linkName has a value linked to it (use index when referring to array)
		bool IsLinked(const std::string& linkName, int index = -1) const;
		// Returns whether a link exists called linkName (use index when referring to array)
		bool HasLink(const std::string& linkName, int index = -1) const;

		// Upload link values to the correct uniforms in shader
		void ApplyLinks() const;

		template<typename T>
		const UniformLink<T>& GetLink(const std::string& linkName, int index = -1) const
		{
			return GetLink<T>(GetLinkId(linkName, index));
		}

		template<typename T>
		UniformLink<T>& GetLink(const std::string& linkName, int index = -1)
		{
			return GetLink<T>(GetLinkId(linkName, index));
		}

		template<typename T>
		const UniformLink<T>& GetLink(id_t linkId) const
		{
			return (const UniformLink<T>&)GetLink(linkId);
		}

		template<typename T>
		UniformLink<T>& GetLink(id_t linkId)
		{
			return (UniformLink<T>&)GetLink(linkId);
		}

		inline UniformLink<int>& Link(const std::string& linkName, const int& value, int index = -1)
		{
			if (index >= 0)
			{
				return LinkIndexed(linkName, value, index);
			}
			const UserUniformLocation& uniform = GetUniformLocation(linkName);
			BLT_ASSERT(Detail::ValidateUniformType<int>(uniform), "Uniform with LinkName {0} does not have type {1} (Type = {2})", linkName, typeid(int).name(), ValueTypeToGLSLString(uniform.Type));
			return (UniformLink<int>&)AddLink(linkName, std::make_unique<UniformLink<int>>(m_Shader->GetShader().Id(), uniform.Location, value));
		}

		inline UniformLink<float>& Link(const std::string& linkName, const float& value, int index = -1)
		{
			if (index >= 0)
			{
				return LinkIndexed(linkName, value, index);
			}
			const UserUniformLocation& uniform = GetUniformLocation(linkName);
			BLT_ASSERT(Detail::ValidateUniformType<float>(uniform), "Uniform with LinkName {0} does not have type {1} (Type = {2})", linkName, typeid(float).name(), ValueTypeToGLSLString(uniform.Type));
			return (UniformLink<float>&)AddLink(linkName, std::make_unique<UniformLink<float>>(m_Shader->GetShader().Id(), uniform.Location, value));
		}

		inline UniformLink<Vector2f>& Link(const std::string& linkName, const Vector2f& value, int index = -1)
		{
			if (index >= 0)
			{
				return LinkIndexed(linkName, value, index);
			}
			const UserUniformLocation& uniform = GetUniformLocation(linkName);
			BLT_ASSERT(Detail::ValidateUniformType<Vector2f>(uniform), "Uniform with LinkName {0} does not have type {1} (Type = {2})", linkName, typeid(Vector2f).name(), ValueTypeToGLSLString(uniform.Type));
			return (UniformLink<Vector2f>&)AddLink(linkName, std::make_unique<UniformLink<Vector2f>>(m_Shader->GetShader().Id(), uniform.Location, value));
		}

		inline UniformLink<Vector3f>& Link(const std::string& linkName, const Vector3f& value, int index = -1)
		{
			if (index >= 0)
			{
				return LinkIndexed(linkName, value, index);
			}
			const UserUniformLocation& uniform = GetUniformLocation(linkName);
			BLT_ASSERT(Detail::ValidateUniformType<Vector3f>(uniform), "Uniform with LinkName {0} does not have type {1} (Type = {2})", linkName, typeid(Vector3f).name(), ValueTypeToGLSLString(uniform.Type));
			return (UniformLink<Vector3f>&)AddLink(linkName, std::make_unique<UniformLink<Vector3f>>(m_Shader->GetShader().Id(), uniform.Location, value));
		}

		inline UniformLink<Vector4f>& Link(const std::string& linkName, const Vector4f& value, int index = -1)
		{
			if (index >= 0)
			{
				return LinkIndexed(linkName, value, index);
			}
			const UserUniformLocation& uniform = GetUniformLocation(linkName);
			BLT_ASSERT(Detail::ValidateUniformType<Vector4f>(uniform), "Uniform with LinkName {0} does not have type {1} (Type = {2})", linkName, typeid(Vector4f).name(), ValueTypeToGLSLString(uniform.Type));
			return (UniformLink<Vector4f>&)AddLink(linkName, std::make_unique<UniformLink<Vector4f>>(m_Shader->GetShader().Id(), uniform.Location, value));
		}

		inline UniformLink<Color>& Link(const std::string& linkName, const Color& value, int index = -1)
		{
			if (index >= 0)
			{
				return LinkIndexed(linkName, value, index);
			}
			const UserUniformLocation& uniform = GetUniformLocation(linkName);
			BLT_ASSERT(Detail::ValidateUniformType<Color>(uniform), "Uniform with LinkName {0} does not have type {1} (Type = {2})", linkName, typeid(Color).name(), ValueTypeToGLSLString(uniform.Type));
			return (UniformLink<Color>&)AddLink(linkName, std::make_unique<UniformLink<Color>>(m_Shader->GetShader().Id(), uniform.Location, value));
		}

		inline UniformLink<Matrix2f>& Link(const std::string& linkName, const Matrix2f& value, int index = -1)
		{
			if (index >= 0)
			{
				return LinkIndexed(linkName, value, index);
			}
			const UserUniformLocation& uniform = GetUniformLocation(linkName);
			BLT_ASSERT(Detail::ValidateUniformType<Matrix2f>(uniform), "Uniform with LinkName {0} does not have type {1} (Type = {2})", linkName, typeid(Matrix2f).name(), ValueTypeToGLSLString(uniform.Type));
			return (UniformLink<Matrix2f>&)AddLink(linkName, std::make_unique<UniformLink<Matrix2f>>(m_Shader->GetShader().Id(), uniform.Location, value));
		}

		inline UniformLink<Matrix3f>& Link(const std::string& linkName, const Matrix3f& value, int index = -1)
		{
			if (index >= 0)
			{
				return LinkIndexed(linkName, value, index);
			}
			const UserUniformLocation& uniform = GetUniformLocation(linkName);
			BLT_ASSERT(Detail::ValidateUniformType<Matrix3f>(uniform), "Uniform with LinkName {0} does not have type {1} (Type = {2})", linkName, typeid(Matrix3f).name(), ValueTypeToGLSLString(uniform.Type));
			return (UniformLink<Matrix3f>&)AddLink(linkName, std::make_unique<UniformLink<Matrix3f>>(m_Shader->GetShader().Id(), uniform.Location, value));
		}

		inline UniformLink<Matrix4f>& Link(const std::string& linkName, const Matrix4f& value, int index = -1)
		{
			if (index >= 0)
			{
				return LinkIndexed(linkName, value, index);
			}
			const UserUniformLocation& uniform = GetUniformLocation(linkName);
			BLT_ASSERT(Detail::ValidateUniformType<Matrix4f>(uniform), "Uniform with LinkName {0} does not have type {1} (Type = {2})", linkName, typeid(Matrix4f).name(), ValueTypeToGLSLString(uniform.Type));
			return (UniformLink<Matrix4f>&)AddLink(linkName, std::make_unique<UniformLink<Matrix4f>>(m_Shader->GetShader().Id(), uniform.Location, value));
		}

		inline UniformLink<AssetHandle<Texture2D>>& Link(const std::string& linkName, const AssetHandle<Texture2D>& value, int index = -1)
		{
			if (index >= 0)
			{
				return LinkIndexed(linkName, value, index);
			}
			const UserUniformLocation& uniform = GetUniformLocation(linkName);
			BLT_ASSERT(Detail::ValidateUniformType<AssetHandle<Texture2D>>(uniform), "Uniform with LinkName {0} does not have type {1} (Type = {2})", linkName, typeid(AssetHandle<Texture2D>).name(), ValueTypeToGLSLString(uniform.Type));
			return (UniformLink<AssetHandle<Texture2D>>&)AddLink(linkName, std::make_unique<UniformLink<AssetHandle<Texture2D>>>(m_Shader->GetShader().Id(), uniform.Location, value));
		}

		inline UniformLink<AssetHandle<Font>>& Link(const std::string& linkName, const AssetHandle<Font>& value, int index = -1)
		{
			if (index >= 0)
			{
				return LinkIndexed(linkName, value, index);
			}
			const UserUniformLocation& uniform = GetUniformLocation(linkName);
			BLT_ASSERT(Detail::ValidateUniformType<AssetHandle<Texture2D>>(uniform), "Uniform with LinkName {0} does not have type {1} (Type = {2})", linkName, typeid(AssetHandle<Font>).name(), ValueTypeToGLSLString(uniform.Type));
			return (UniformLink<AssetHandle<Font>>&)AddLink(linkName, std::make_unique<UniformLink<AssetHandle<Font>>>(m_Shader->GetShader().Id(), uniform.Location, value));
		}

		template<typename FuncT, typename R = typename std::result_of<FuncT()>::type>
		UniformLink<std::function<R()>>& Link(const std::string& linkName, const FuncT& value, int index = -1)
		{
			if (index >= 0)
			{
				return LinkIndexedFunction(linkName, value, index);
			}
			const UserUniformLocation& uniform = GetUniformLocation(linkName);
			BLT_ASSERT(Detail::ValidateUniformType<R>(uniform), "Uniform with LinkName {0} does not have type {1} (Type = {2})", linkName, typeid(R).name(), ValueTypeToGLSLString(uniform.Type));
			return (UniformLink<std::function<R()>>&)AddLink(linkName, std::make_unique<UniformLink<std::function<R()>>>(m_Shader->GetShader().Id(), uniform.Location, value));
		}

		template<typename T>
		UniformLink<T>& LinkIndexed(const std::string& linkName, const T& value, int index)
		{
			BLT_ASSERT(HasLink(linkName, 0), "Uniform {} is not an array", linkName);
			BLT_ASSERT(HasLink(linkName, index), "Uniform index {} out of range", index);
			return Link(linkName + '[' + std::to_string(index) + ']', value);
		}

		template<typename FuncT, typename R = typename std::result_of<FuncT()>::type>
		UniformLink<std::function<R()>>& LinkIndexedFunction(const std::string& linkName, const FuncT& value, int index)
		{
			BLT_ASSERT(HasLink(linkName, 0), "Uniform {} is not an array", linkName);
			BLT_ASSERT(HasLink(linkName, index), "Uniform index {} out of range", index);
			return Link(linkName + '[' + std::to_string(index) + ']', value);
		}

	private:
		const UserUniformLocation& GetUniformLocation(const std::string& linkName) const;
		UniformLinkContainer& AddLink(const std::string& linkName, std::unique_ptr<UniformLinkContainer>&& link);
		void CopyLinksTo(ShaderLinkContext& other) const;
	};

}