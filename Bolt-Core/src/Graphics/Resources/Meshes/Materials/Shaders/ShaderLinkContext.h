#pragma once
#include "ShaderInstance.h"
#include "UniformLink.h"

#include "Core/Math/Maths.h"
#include "Graphics/Resources/Textures/Texture2D.h"
#include "Graphics/Resources/Textures/Fonts/Font.h"

namespace Bolt
{

	// Represents a shader and the uniform values associated with it.
	// Makes up the main part of Materials
	class BLT_API ShaderLinkContext
	{
	public:
		struct BLT_API LinkId
		{
		public:
			bool IsLinked = false;
			id_t UniformIndex = 0;
			id_t LinkIndex = 0;
			bool RequiresLink = true;
		};

	private:
		std::shared_ptr<ShaderInstance> m_Shader;
		std::vector<std::unique_ptr<UniformLinkContainer>> m_Links;
		std::unordered_map<blt::string, LinkId> m_UserUniformLinks;

	public:
		ShaderLinkContext(const std::shared_ptr<ShaderInstance>& shaderInstance);
		ShaderLinkContext(const ShaderLinkContext& other);
		ShaderLinkContext& operator=(const ShaderLinkContext& other);
		ShaderLinkContext(ShaderLinkContext&& other) = default;
		ShaderLinkContext& operator=(ShaderLinkContext&& other) = default;
		~ShaderLinkContext() = default;

		const ShaderInstance& GetShaderInstance() const;
		// Return a unique identifier for the link represented by linkName (use index when linking array)
		id_t GetLinkId(const blt::string& linkName, int index = -1) const;
		// Returns the container with the value for the link with linkName (use index when referring to array)
		const UniformLinkContainer& GetLink(const blt::string& linkName, int index = -1) const;
		// Returns the container with the value for the link with linkName (use index when referring to array)
		UniformLinkContainer& GetLink(const blt::string& linkName, int index = -1);
		// Returns the container with the value for the link with id linkId
		const UniformLinkContainer& GetLink(id_t linkId) const;
		// Returns the container with the value for the link with id linkId
		UniformLinkContainer& GetLink(id_t linkId);
		// Returns whether the link with linkName has a value linked to it (use index when referring to array)
		bool IsLinked(const blt::string& linkName, int index = -1) const;
		// Returns whether a link exists called linkName (use index when referring to array)
		bool HasLink(const blt::string& linkName, int index = -1) const;

		// Upload link values to the correct uniforms in shader
		void ApplyLinks() const;

		template<typename T>
		const UniformLink<T>& GetLink(const blt::string& linkName, int index = -1) const
		{
			return GetLink<T>(GetLinkId(linkName, index));
		}

		template<typename T>
		UniformLink<T>& GetLink(const blt::string& linkName, int index = -1)
		{
			return GetLink<T>(GetLinkId(linkName, index));
		}

		template<typename T>
		const UniformLink<T>& GetLink(id_t linkId, int index = -1) const
		{
			return (const UniformLink<T>&)GetLink(linkId, index);
		}

		template<typename T>
		UniformLink<T>& GetLink(id_t linkId, int index = -1)
		{
			return (UniformLink<T>&)GetLink(linkId, index);
		}

		template<typename T>
		UniformLink<T>& Link(const blt::string& linkName, const T& value, int index = -1)
		{
			BLT_ASSERT(false, "Unsupported uniform type {}", typeid(T).name());
			return UniformLink<T>();
		}

		template<>
		UniformLink<int>& Link(const blt::string& linkName, const int& value, int index)
		{
			if (index >= 0)
			{
				return Link(linkName + '[' + std::to_string(index) + ']', value);
			}
			const UserUniformLocation& uniform = GetUniformLocation(linkName);
			BLT_ASSERT(ValidateUniformType<int>(uniform), "Uniform with LinkName {0} does not have type {1} (Type = {2})", linkName, typeid(int).name(), ValueTypeToGLSLString(uniform.Type));
			return (UniformLink<int>&)AddLink(linkName, std::make_unique<UniformLink<int>>(m_Shader->GetShader().Id(), uniform.Location, value));
		}

		template<>
		UniformLink<float>& Link(const blt::string& linkName, const float& value, int index)
		{
			if (index >= 0)
			{
				return Link(linkName + '[' + std::to_string(index) + ']', value);
			}
			const UserUniformLocation& uniform = GetUniformLocation(linkName);
			BLT_ASSERT(ValidateUniformType<float>(uniform), "Uniform with LinkName {0} does not have type {1} (Type = {2})", linkName, typeid(float).name(), ValueTypeToGLSLString(uniform.Type));
			return (UniformLink<float>&)AddLink(linkName, std::make_unique<UniformLink<float>>(m_Shader->GetShader().Id(), uniform.Location, value));
		}

		template<>
		UniformLink<Vector2f>& Link(const blt::string& linkName, const Vector2f& value, int index)
		{
			if (index >= 0)
			{
				return Link(linkName + '[' + std::to_string(index) + ']', value);
			}
			const UserUniformLocation& uniform = GetUniformLocation(linkName);
			BLT_ASSERT(ValidateUniformType<Vector2f>(uniform), "Uniform with LinkName {0} does not have type {1} (Type = {2})", linkName, typeid(Vector2f).name(), ValueTypeToGLSLString(uniform.Type));
			return (UniformLink<Vector2f>&)AddLink(linkName, std::make_unique<UniformLink<Vector2f>>(m_Shader->GetShader().Id(), uniform.Location, value));
		}

		template<>
		UniformLink<Vector3f>& Link(const blt::string& linkName, const Vector3f& value, int index)
		{
			if (index >= 0)
			{
				return Link(linkName + '[' + std::to_string(index) + ']', value);
			}
			const UserUniformLocation& uniform = GetUniformLocation(linkName);
			BLT_ASSERT(ValidateUniformType<Vector3f>(uniform), "Uniform with LinkName {0} does not have type {1} (Type = {2})", linkName, typeid(Vector3f).name(), ValueTypeToGLSLString(uniform.Type));
			return (UniformLink<Vector3f>&)AddLink(linkName, std::make_unique<UniformLink<Vector3f>>(m_Shader->GetShader().Id(), uniform.Location, value));
		}

		template<>
		UniformLink<Vector4f>& Link(const blt::string& linkName, const Vector4f& value, int index)
		{
			if (index >= 0)
			{
				return Link(linkName + '[' + std::to_string(index) + ']', value);
			}
			const UserUniformLocation& uniform = GetUniformLocation(linkName);
			BLT_ASSERT(ValidateUniformType<Vector4f>(uniform), "Uniform with LinkName {0} does not have type {1} (Type = {2})", linkName, typeid(Vector4f).name(), ValueTypeToGLSLString(uniform.Type));
			return (UniformLink<Vector4f>&)AddLink(linkName, std::make_unique<UniformLink<Vector4f>>(m_Shader->GetShader().Id(), uniform.Location, value));
		}

		template<>
		UniformLink<Color>& Link(const blt::string& linkName, const Color& value, int index)
		{
			if (index >= 0)
			{
				return Link(linkName + '[' + std::to_string(index) + ']', value);
			}
			const UserUniformLocation& uniform = GetUniformLocation(linkName);
			BLT_ASSERT(ValidateUniformType<Color>(uniform), "Uniform with LinkName {0} does not have type {1} (Type = {2})", linkName, typeid(Color).name(), ValueTypeToGLSLString(uniform.Type));
			return (UniformLink<Color>&)AddLink(linkName, std::make_unique<UniformLink<Color>>(m_Shader->GetShader().Id(), uniform.Location, value));
		}

		template<>
		UniformLink<Matrix2f>& Link(const blt::string& linkName, const Matrix2f& value, int index)
		{
			if (index >= 0)
			{
				return Link(linkName + '[' + std::to_string(index) + ']', value);
			}
			const UserUniformLocation& uniform = GetUniformLocation(linkName);
			BLT_ASSERT(ValidateUniformType<Matrix2f>(uniform), "Uniform with LinkName {0} does not have type {1} (Type = {2})", linkName, typeid(Matrix2f).name(), ValueTypeToGLSLString(uniform.Type));
			return (UniformLink<Matrix2f>&)AddLink(linkName, std::make_unique<UniformLink<Matrix2f>>(m_Shader->GetShader().Id(), uniform.Location, value));
		}

		template<>
		UniformLink<Matrix3f>& Link(const blt::string& linkName, const Matrix3f& value, int index)
		{
			if (index >= 0)
			{
				return Link(linkName + '[' + std::to_string(index) + ']', value);
			}
			const UserUniformLocation& uniform = GetUniformLocation(linkName);
			BLT_ASSERT(ValidateUniformType<Matrix3f>(uniform), "Uniform with LinkName {0} does not have type {1} (Type = {2})", linkName, typeid(Matrix3f).name(), ValueTypeToGLSLString(uniform.Type));
			return (UniformLink<Matrix3f>&)AddLink(linkName, std::make_unique<UniformLink<Matrix3f>>(m_Shader->GetShader().Id(), uniform.Location, value));
		}

		template<>
		UniformLink<Matrix4f>& Link(const blt::string& linkName, const Matrix4f& value, int index)
		{
			if (index >= 0)
			{
				return Link(linkName + '[' + std::to_string(index) + ']', value);
			}
			const UserUniformLocation& uniform = GetUniformLocation(linkName);
			BLT_ASSERT(ValidateUniformType<Matrix4f>(uniform), "Uniform with LinkName {0} does not have type {1} (Type = {2})", linkName, typeid(Matrix4f).name(), ValueTypeToGLSLString(uniform.Type));
			return (UniformLink<Matrix4f>&)AddLink(linkName, std::make_unique<UniformLink<Matrix4f>>(m_Shader->GetShader().Id(), uniform.Location, value));
		}

		template<>
		UniformLink<ResourcePtr<Texture2D>>& Link(const blt::string& linkName, const ResourcePtr<Texture2D>& value, int index)
		{
			if (index >= 0)
			{
				return Link(linkName + '[' + std::to_string(index) + ']', value);
			}
			const UserUniformLocation& uniform = GetUniformLocation(linkName);
			BLT_ASSERT(ValidateUniformType<ResourcePtr<Texture2D>>(uniform), "Uniform with LinkName {0} does not have type {1} (Type = {2})", linkName, typeid(ResourcePtr<Texture2D>).name(), ValueTypeToGLSLString(uniform.Type));
			return (UniformLink<ResourcePtr<Texture2D>>&)AddLink(linkName, std::make_unique<UniformLink<ResourcePtr<Texture2D>>>(m_Shader->GetShader().Id(), uniform.Location, value, uniform.Index));
		}

		template<>
		UniformLink<ResourcePtr<Font>>& Link(const blt::string& linkName, const ResourcePtr<Font>& value, int index)
		{
			if (index >= 0)
			{
				return Link(linkName + '[' + std::to_string(index) + ']', value);
			}
			const UserUniformLocation& uniform = GetUniformLocation(linkName);
			BLT_ASSERT(ValidateUniformType<ResourcePtr<Texture2D>>(uniform), "Uniform with LinkName {0} does not have type {1} (Type = {2})", linkName, typeid(ResourcePtr<Font>).name(), ValueTypeToGLSLString(uniform.Type));
			return (UniformLink<ResourcePtr<Font>>&)AddLink(linkName, std::make_unique<UniformLink<ResourcePtr<Font>>>(m_Shader->GetShader().Id(), uniform.Location, value, uniform.Index));
		}

	private:
		const UserUniformLocation& GetUniformLocation(const blt::string& linkName) const;
		UniformLinkContainer& AddLink(const blt::string& linkName, std::unique_ptr<UniformLinkContainer>&& link);
		void CopyLinksTo(ShaderLinkContext& other) const;

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
		bool ValidateUniformType<ResourcePtr<Texture2D>>(const UserUniformLocation& uniform) const
		{
			return uniform.Type == ValueType::Texture2D;
		}

	};

}