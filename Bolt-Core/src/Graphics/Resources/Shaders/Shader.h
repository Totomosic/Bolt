#pragma once
#include "Bolt-Core.h"
#include "..\Resource.h"
#include "..\ResourcePtr.h"
#include "..\..\GLshared.h"
#include "Compiler\__Compiler__.h"

namespace Bolt
{

	struct Material;
	struct LightSource;

	BLT_API enum class UniformType
	{
		None,
		Bool,
		Int,
		Float,
		Double,
		Vector2f,
		Vector2i,
		Vector3f,
		Vector3i,
		Vector4f,
		Vector4i,
		Matrix3f,
		Matrix4f,
		Sampler2D,
		Sampler3D,
		SamplerCube
	};

	class BLT_API Shader : public Resource, public GLshared
	{
	public:
		struct BLT_API UniformVariable
		{
		public:
			blt::string Name;
			int Location;
			UniformType Type;
		};

		static blt::string MODEL_MATRIX_NAME;
		static blt::string VIEW_MATRIX_NAME;
		static blt::string PROJECTION_MATRIX_NAME;
		static blt::string BASE_COLOR_NAME;

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
		std::unordered_map<id_t, int> m_ShaderConcepts;

		mutable std::unordered_map<blt::string, int> m_UniformLocations;
		std::unordered_map<int, UniformVariable> m_UniformVariables;
		std::vector<UniformVariable> m_TextureSamplers;

		int m_ModelMatrixLocation;
		int m_ViewMatrixLocation;
		int m_ProjectionMatrixLocation;
		int m_BaseColorLocation;

	public:
		Shader(const blt::string& vertexSource, const blt::string& fragmentSource);
		Shader(const blt::string& vertexSource, const blt::string& geometrySource, const blt::string& fragmentSource);

	public:
		id_t ID() const;
		const UniformVariable& GetVariable(const blt::string& location) const;
		const UniformVariable& GetTextureSampler(int index) const;
		const std::vector<UniformVariable>& GetAllTextureSamplers() const;
		blt::vector<UniformVariable> GetAllUniforms() const;
		int UniformVariableCount() const;
		int TextureSamplerCount() const;

		void Bind() const;
		void Unbind() const;

		void SetModelMatrix(const Matrix4f& matrix) const;
		void SetViewMatrix(const Matrix4f& matrix) const;
		void SetProjectionMatrix(const Matrix4f& matrix) const;
		void SetColor(const Color& color) const;
		void SetMaterial(const Material& material) const;
		void SetLights(const std::vector<LightSource>& lights) const;

		void SetUniform(const blt::string& location, bool value) const;
		void SetUniform(const blt::string& location, int value) const;
		void SetUniform(const blt::string& location, uint value) const;
		void SetUniform(const blt::string& location, float value) const;
		void SetUniform(const blt::string& location, double value) const;
		void SetUniform(const blt::string& location, const Vector2f& value) const;
		void SetUniform(const blt::string& location, const Vector2i& value) const;
		void SetUniform(const blt::string& location, const Vector3f& value) const;
		void SetUniform(const blt::string& location, const Vector3i& value) const;
		void SetUniform(const blt::string& location, const Vector4f& value) const;
		void SetUniform(const blt::string& location, const Vector4i& value) const;
		void SetUniform(const blt::string& location, const Color& value) const;
		void SetUniform(const blt::string& location, const Matrix3f& value) const;
		void SetUniform(const blt::string& location, const Matrix4f& value) const;
		void SetUniform(const blt::string& location, const Quaternion& value) const;

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
		void SetUniform(int location, const Matrix3f& value) const;
		void SetUniform(int location, const Matrix4f& value) const;
		void SetUniform(int location, const Quaternion& value) const;

		template<typename T>
		void SetUniform(const blt::string& location, const T& value) const
		{
			SetUniform(location, value);
		}

		std::unique_ptr<Resource> Clone() const override;

		friend class Initializer;

	public:
		static std::unique_ptr<Shader> FromFile(const Filepath& shaderFile);
		static std::unique_ptr<Shader> FromFile(const Filepath& vertexFile, const Filepath& fragmentFile);
		static std::unique_ptr<Shader> FromFile(const Filepath& vertexFile, const Filepath& geometryFile, const Filepath& fragmentFile);
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
		int GetUniformLocation(const blt::string& lcoation) const;
		void ValidateUploadValue(int location, UniformType type) const;

		void ReflectSource(const blt::string& source);
		UniformType DetermineVariableType(const blt::string& typeName) const;

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