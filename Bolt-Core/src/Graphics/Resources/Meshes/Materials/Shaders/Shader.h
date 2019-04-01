#pragma once
#include "Bolt-Core.h"
#include "..\..\..\Resource.h"
#include "..\..\..\ResourcePtr.h"
#include "..\..\..\..\GLshared.h"
#include "Compiler\__Compiler__.h"

namespace Bolt
{

	class BLT_API Shader : public Resource, public GLshared
	{
	private:
		static ResourcePtr<const Shader> s_DefaultColorShader;
		static ResourcePtr<const Shader> s_DefaultTextureShader;
		static ResourcePtr<const Shader> s_DefaultFontShader;
		static ResourcePtr<const Shader> s_DefaultSkyboxShader;
		static ResourcePtr<const Shader> s_SpriteTextureShader;

		static ResourcePtr<const Shader> s_LightingColorShader;
		static ResourcePtr<const Shader> s_LightingTextureShader;

	private:
		id_t m_Id;

	public:
		Shader(const blt::string& vertexSource, const blt::string& fragmentSource);
		Shader(const blt::string& vertexSource, const blt::string& geometrySource, const blt::string& fragmentSource);
		Shader(const Shader& other) = delete;
		Shader& operator=(const Shader& other) = delete;
		Shader(Shader&& other);
		Shader& operator=(Shader&& other);
		~Shader() override;

		id_t Id() const;

		void Bind() const;
		void Unbind() const;

		void SetUniform(int location, bool value) const;
		void SetUniform(int location, int value) const;
		void SetUniform(int location, uint value) const;
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

		friend class Initializer;
		friend class ShaderInstance;

	public:
		static std::unique_ptr<Shader> FromFile(const Filepath& shaderFile);
		static std::unique_ptr<Shader> FromFile(const Filepath& vertexFile, const Filepath& fragmentFile);
		static std::unique_ptr<Shader> FromFile(const Filepath& vertexFile, const Filepath& geometryFile, const Filepath& fragmentFile);
		static std::unique_ptr<Shader> FromSource(const blt::string& source);
		static std::unique_ptr<Shader> FromSource(const blt::string& vertexSource, const blt::string& fragmentSource);
		static std::unique_ptr<Shader> FromSource(const blt::string& vertexSource, const blt::string& geometrySource, const blt::string& fragmentSource);

		static ResourcePtr<const Shader> DefaultColor();
		static ResourcePtr<const Shader> DefaultTexture();
		static ResourcePtr<const Shader> DefaultFont();
		static ResourcePtr<const Shader> DefaultSkybox();
		static ResourcePtr<const Shader> SpriteTexture();

		static ResourcePtr<const Shader> LightingColor();
		static ResourcePtr<const Shader> LightingTexture();

	private:
		void Create();
		void Finalise(id_t* shaders, int count);
		id_t AddShader(const blt::string& shaderSource, GLenum shaderType);
		int GetUniformLocation(const blt::string& location) const;

	private:	
		static void Initialize();
		static const Shader* CreateDefaultColorShader();
		static const Shader* CreateDefaultTextureShader();
		static const Shader* CreateDefaultFontShader();
		static const Shader* CreateDefaultSkyboxShader();
		static const Shader* CreateSpriteTextureShader();

		static const Shader* CreateLightingColorShader();
		static const Shader* CreateLightingTextureShader();

	};

}