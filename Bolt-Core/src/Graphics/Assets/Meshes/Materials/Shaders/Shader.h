#pragma once
#include "BoltLib/Math/Maths.h"
#include "BoltLib/Color.h"
#include "BoltLib/IO/Filesystem.h"
#include "Graphics/Assets/Resource.h"
#include "Graphics/Assets/AssetHandle.h"

namespace Bolt
{

	class BLT_API Shader : public Resource
	{
	private:
		id_t m_Id;

	public:
		Shader(const std::string& vertexSource, const std::string& fragmentSource);
		Shader(const std::string& vertexSource, const std::string& geometrySource, const std::string& fragmentSource);
		Shader(const Shader& other) = delete;
		Shader& operator=(const Shader& other) = delete;
		Shader(Shader&& other) noexcept;
		Shader& operator=(Shader&& other) noexcept;
		~Shader() override;

		id_t Id() const;

		void Bind() const;
		void Unbind() const;

		void SetUniform(int location, bool value) const;
		void SetUniform(int location, int value) const;
		void SetUniform(int location, uint32_t value) const;
		void SetUniform(int location, float value) const;
		void SetUniform(int location, double value) const;
		void SetUniform(int location, const Vector2f& value) const;
		void SetUniform(int location, const Vector2i& value) const;
		void SetUniform(int location, const Vector3f& value) const;
		void SetUniform(int location, const Vector3i& value) const;
		void SetUniform(int location, const Vector4f& value) const;
		void SetUniform(int location, const Vector4i& value) const;
		void SetUniform(int location, const Color& value) const;
		void SetUniform(int location, const Matrix2f& value) const;
		void SetUniform(int location, const Matrix3f& value) const;
		void SetUniform(int location, const Matrix4f& value) const;
		void SetUniform(int location, const Quaternion& value) const;

		std::unique_ptr<Resource> Clone() const override;

		friend class ShaderInstance;

	public:
		static std::unique_ptr<Shader> FromFile(const FilePath& shaderFile);
		static std::unique_ptr<Shader> FromFile(const FilePath& vertexFile, const FilePath& fragmentFile);
		static std::unique_ptr<Shader> FromFile(const FilePath& vertexFile, const FilePath& geometryFile, const FilePath& fragmentFile);
		static std::unique_ptr<Shader> FromSource(const std::string& source);
		static std::unique_ptr<Shader> FromSource(const std::string& vertexSource, const std::string& fragmentSource);
		static std::unique_ptr<Shader> FromSource(const std::string& vertexSource, const std::string& geometrySource, const std::string& fragmentSource);

	private:
		void Create();
		void Finalise(id_t* shaders, int count);
		id_t AddShader(const std::string& shaderSource, GLenum shaderType);
		int GetUniformLocation(const std::string& location) const;

	};

}