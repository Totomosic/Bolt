#include "Types.h"

#include "Shader.h"
#include "Graphics\Resources\ResourceManager.h"
#include "..\Material.h"
#include "Engine\Scene\Lighting\LightSource.h"
#include "Engine\Renderer\GLState.h"

namespace Bolt
{

	Matrix3f CalculateMatrix3(const TextureBounds& bounds)
	{
		float width = bounds.MaxX - bounds.MinX;
		float height = bounds.MaxY - bounds.MinY;
		Matrix3f result = Matrix3f::Translation(bounds.MinX, bounds.MinY, 0) * Matrix3f::Scale(width, height, 1);
		return result;
	}

#ifdef BLT_DEBUG
	#define BLT_SHADER_VALIDATE_VALUE(...) ValidateUploadValue(__VA_ARGS__)
#else
	#define BLT_SHADER_VALIDATE_VALUE(...)
#endif

	blt::string Shader::MODEL_MATRIX_NAME = "u_ModelMatrix";
	blt::string Shader::VIEW_MATRIX_NAME = "u_ViewMatrix";
	blt::string Shader::PROJECTION_MATRIX_NAME = "u_ProjectionMatrix";
	blt::string Shader::BASE_COLOR_NAME = "Material.MeshColor";

	ResourcePtr<const Shader> Shader::s_DefaultColorShader = nullptr;
	ResourcePtr<const Shader> Shader::s_DefaultTextureShader = nullptr;
	ResourcePtr<const Shader> Shader::s_DefaultFontShader = nullptr;
	ResourcePtr<const Shader> Shader::s_DefaultSkyboxShader = nullptr;
	ResourcePtr<const Shader> Shader::s_SpriteTextureShader = nullptr;

	ResourcePtr<const Shader> Shader::s_LightingColorShader = nullptr;
	ResourcePtr<const Shader> Shader::s_LightingTextureShader = nullptr;

	Shader::Shader(const blt::string& vertexSource, const blt::string& fragmentSource) : Resource(), GLshared(),
		m_Id(0)
	{
		Create();
		id_t vertex = AddShader(vertexSource, GL_VERTEX_SHADER);
		id_t fragment = AddShader(fragmentSource, GL_FRAGMENT_SHADER);
		id_t arr[2] = { vertex, fragment };
		Finalise(arr, 2);
	}

	Shader::Shader(const blt::string& vertexSource, const blt::string& geometrySource, const blt::string& fragmentSource) : Resource(), GLshared(),
		m_Id(0)
	{
		Create();
		id_t vertex = AddShader(vertexSource, GL_VERTEX_SHADER);
		id_t geometry = AddShader(geometrySource, GL_GEOMETRY_SHADER);
		id_t fragment = AddShader(fragmentSource, GL_FRAGMENT_SHADER);
		id_t arr[3] = { vertex, geometry, fragment };
		Finalise(arr, 3);
	}

	Shader::Shader(Shader&& other)
		: m_Id(other.m_Id)
	{
		other.m_Id = 0;
	}

	Shader& Shader::operator=(Shader&& other)
	{
		id_t id = m_Id;
		m_Id = other.m_Id;
		other.m_Id = id;
		return *this;
	}

	Shader::~Shader()
	{
		if (m_Id != 0)
		{
			GL_CALL(glDeleteProgram(m_Id));
		}
	}

	id_t Shader::Id() const
	{
		return m_Id;
	}

	void Shader::Bind() const
	{
		GL_CALL(glUseProgram(m_Id));
	}

	void Shader::Unbind() const
	{
		GL_CALL(glUseProgram(0));
	}

	void Shader::SetModelMatrix(const Matrix4f& matrix) const
	{
		SetUniform(m_ModelMatrixLocation, matrix);
	}

	void Shader::SetViewMatrix(const Matrix4f& matrix) const
	{
		SetUniform(m_ViewMatrixLocation, matrix);
	}

	void Shader::SetProjectionMatrix(const Matrix4f& matrix) const
	{
		SetUniform(m_ProjectionMatrixLocation, matrix);
	}

	void Shader::SetColor(const Color& color) const
	{
		SetUniform(m_BaseColorLocation, color);
	}

	void Shader::SetMaterial(const Material& material) const
	{
		SetColor(material.BaseColor);
	}

	void Shader::SetLights(const std::vector<LightSource>& lights) const
	{
		
	}

	void Shader::SetUniform(int location, bool value) const
	{
		Bind();		
		GL_CALL(glUniform1i(location, value));
	}

	void Shader::SetUniform(int location, int value) const
	{
		Bind();		
		GL_CALL(glUniform1i(location, value));
	}

	void Shader::SetUniform(int location, uint value) const
	{
		Bind();		
		GL_CALL(glUniform1i(location, (int)value));
	}

	void Shader::SetUniform(int location, float value) const
	{
		Bind();		
		GL_CALL(glUniform1f(location, value));
	}

	void Shader::SetUniform(int location, double value) const
	{
		Bind();
		GL_CALL(glUniform1d(location, value));
	}

	void Shader::SetUniform(int location, const Vector2f& value) const
	{
		Bind();
		GL_CALL(glUniform2f(location, value.x, value.y));
	}

	void Shader::SetUniform(int location, const Vector2i& value) const
	{
		Bind();
		GL_CALL(glUniform2i(location, value.x, value.y));
	}

	void Shader::SetUniform(int location, const Vector3f& value) const
	{
		Bind();
		GL_CALL(glUniform3f(location, value.x, value.y, value.z));
	}

	void Shader::SetUniform(int location, const Vector3i& value) const
	{
		Bind();
		GL_CALL(glUniform3i(location, value.x, value.y, value.z));
	}

	void Shader::SetUniform(int location, const Vector4f& value) const
	{
		Bind();
		GL_CALL(glUniform4f(location, value.x, value.y, value.z, value.w));
	}

	void Shader::SetUniform(int location, const Vector4i& value) const
	{
		Bind();
		GL_CALL(glUniform4i(location, value.x, value.y, value.z, value.w));
	}

	void Shader::SetUniform(int location, const Color& value) const
	{
		Bind();
		GL_CALL(glUniform4f(location, value.r, value.g, value.b, value.a));
	}

	void Shader::SetUniform(int location, const Matrix2f& value) const
	{
		Bind();
		GL_CALL(glUniformMatrix2fv(location, 1, GL_FALSE, value.values));
	}

	void Shader::SetUniform(int location, const Matrix3f& value) const
	{
		Bind();
		GL_CALL(glUniformMatrix3fv(location, 1, GL_FALSE, value.values));
	}

	void Shader::SetUniform(int location, const Matrix4f& value) const
	{
		Bind();
		GL_CALL(glUniformMatrix4fv(location, 1, GL_FALSE, value.values));
	}

	void Shader::SetUniform(int location, const Quaternion& value) const
	{
		Bind();
		GL_CALL(glUniform4f(location, value.x, value.y, value.z, value.w));
	}

	std::unique_ptr<Resource> Shader::Clone() const
	{
		BLT_ASSERT(false, "Currently unable to clone Shaders");
		return nullptr;
	}

	std::unique_ptr<Shader> Shader::FromFile(const Filepath& shaderFile)
	{
		File f = Filesystem::Open(shaderFile, OpenMode::Read);
		blt::string fileData = f.ReadText();
		return FromSource(fileData);
	}

	std::unique_ptr<Shader> Shader::FromFile(const Filepath& vertexFile, const Filepath& fragmentFile)
	{
		File v = Filesystem::Open(vertexFile, OpenMode::Read);
		File f = Filesystem::Open(fragmentFile, OpenMode::Read);
		return FromSource(v.ReadText(), f.ReadText());
	}

	std::unique_ptr<Shader> Shader::FromFile(const Filepath& vertexFile, const Filepath& geometryFile, const Filepath& fragmentFile)
	{
		File v = Filesystem::Open(vertexFile, OpenMode::Read);
		File g = Filesystem::Open(geometryFile, OpenMode::Read);
		File f = Filesystem::Open(fragmentFile, OpenMode::Read);
		return FromSource(v.ReadText(), g.ReadText(), f.ReadText());
	}

	std::unique_ptr<Shader> Shader::FromSource(const blt::string& source)
	{
		constexpr int NONE_SHADER_TYPE = -1;
		constexpr int VERTEX_SHADER_TYPE = 0;
		constexpr int FRAGMENT_SHADER_TYPE = 2;
		constexpr int GEOMETRY_SHADER_TYPE = 1;

		std::stringstream ss[3];
		int currentType = NONE_SHADER_TYPE;
		bool isGeometryShader = false;

		blt::string fileData = source;
		std::vector<blt::string> lines = fileData.split('\n');
		for (blt::string& line : lines)
		{
			if (line.find("#shader") != blt::string::npos)
			{
				if (line.find("vertex") != blt::string::npos)
				{
					currentType = VERTEX_SHADER_TYPE;
				}
				else if (line.find("fragment") != blt::string::npos)
				{
					currentType = FRAGMENT_SHADER_TYPE;
				}
				else if (line.find("geometry") != blt::string::npos)
				{
					currentType = GEOMETRY_SHADER_TYPE;
					isGeometryShader = true;
				}
			}
			else
			{
				ss[currentType] << line << '\n';
			}
		}
		blt::string sources[3] = { ss[0].str(), ss[1].str(), ss[2].str() };
		return (isGeometryShader) ? FromSource(sources[0], sources[1], sources[2]) : FromSource(sources[0], sources[2]);
	}

	std::unique_ptr<Shader> Shader::FromSource(const blt::string& vertexSource, const blt::string& fragmentSource)
	{
		blt::string sources[3] = { vertexSource, "", fragmentSource };
		ComShaderProgram program = ShaderCompiler::Compile(sources, "");
		return std::make_unique<Shader>(program.Sources[0], program.Sources[2]);
	}

	std::unique_ptr<Shader> Shader::FromSource(const blt::string& vertexSource, const blt::string& geometrySource, const blt::string& fragmentSource)
	{
		blt::string sources[3] = { vertexSource, geometrySource, fragmentSource };
		ComShaderProgram program = ShaderCompiler::Compile(sources, "");
		return std::make_unique<Shader>(program.Sources[0], program.Sources[1], program.Sources[2]);
	}

	ResourcePtr<const Shader> Shader::DefaultColor()
	{
		return s_DefaultColorShader;
	}

	ResourcePtr<const Shader> Shader::DefaultTexture()
	{
		return s_DefaultTextureShader;
	}

	ResourcePtr<const Shader> Shader::DefaultFont()
	{
		return s_DefaultFontShader;
	}

	ResourcePtr<const Shader> Shader::DefaultSkybox()
	{
		return s_DefaultSkyboxShader;
	}

	ResourcePtr<const Shader> Shader::SpriteTexture()
	{
		return s_SpriteTextureShader;
	}

	ResourcePtr<const Shader> Shader::LightingColor()
	{
		return s_LightingColorShader;
	}

	ResourcePtr<const Shader> Shader::LightingTexture()
	{
		return s_LightingTextureShader;
	}

	void Shader::Create()
	{
		m_Id = GL_CALL(glCreateProgram());
	}

	void Shader::Finalise(id_t* shaders, int count)
	{
		GL_CALL(glLinkProgram(m_Id));
		int result;
		GL_CALL(glGetProgramiv(m_Id, GL_LINK_STATUS, &result));
		if (result == GL_FALSE)
		{
			int length;
			GL_CALL(glGetProgramiv(m_Id, GL_INFO_LOG_LENGTH, &length));
			std::vector<char> error(length);
			GL_CALL(glGetProgramInfoLog(m_Id, length, &length, &error[0]));
			blt::string errorMsg(&error[0]);
			BLT_CORE_ERROR("Shader Failed To Link");
			BLT_CORE_ERROR(errorMsg);
		}
		for (int i = 0; i < count; i++)
		{
			GL_CALL(glDetachShader(m_Id, shaders[i]));
			GL_CALL(glDeleteShader(shaders[i]));
		}
		m_ModelMatrixLocation = GetUniformLocation(Shader::MODEL_MATRIX_NAME);
		m_ViewMatrixLocation = GetUniformLocation(Shader::VIEW_MATRIX_NAME);
		m_ProjectionMatrixLocation = GetUniformLocation(Shader::PROJECTION_MATRIX_NAME);
		m_BaseColorLocation = GetUniformLocation(Shader::BASE_COLOR_NAME);
	}

	id_t Shader::AddShader(const blt::string& source, GLenum shaderType)
	{
		id_t shader = GL_CALL(glCreateShader(shaderType));
		const char* str = source.c_str();
		GL_CALL(glShaderSource(shader, 1, &str, nullptr));
		GL_CALL(glCompileShader(shader));

		int result;
		GL_CALL(glGetShaderiv(shader, GL_COMPILE_STATUS, &result));
		if (result == GL_FALSE)
		{
			int length;
			GL_CALL(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length));
			std::vector<char> error(length);
			GL_CALL(glGetShaderInfoLog(shader, length, &length, &error[0]));
			blt::string errorMessage(&error[0]);
			GL_CALL(glDeleteShader(shader));
			BLT_CORE_ERROR(errorMessage);
			return 0;
		}
		GL_CALL(glAttachShader(m_Id, shader));
		return shader;
	}

	int Shader::GetUniformLocation(const blt::string& location) const
	{
		Bind();
		int loc = GL_CALL(glGetUniformLocation(m_Id, location.c_str()));
		return loc;
	}

	void Shader::Initialize()
	{
		blt::string materialHeaderSource =
#include "Source\Headers\BoltMaterials.glh"
			;
		blt::string lightingHeaderSource =
#include "Source\Headers\BoltLighting.glh"
			;
		ShaderCompiler::AddHeader("BoltMaterials.glh", materialHeaderSource);
		ShaderCompiler::AddHeader("BoltLighting.glh", lightingHeaderSource);

		s_DefaultColorShader = CreateDefaultColorShader();
		s_DefaultTextureShader = CreateDefaultTextureShader();
		s_DefaultFontShader = CreateDefaultFontShader();
		s_DefaultSkyboxShader = CreateDefaultSkyboxShader();
		s_SpriteTextureShader = CreateSpriteTextureShader();

		s_LightingColorShader = CreateLightingColorShader();
		s_LightingTextureShader = CreateLightingTextureShader();
	}

	const Shader* Shader::CreateDefaultColorShader()
	{
		blt::string vSource =
#include "Source\DefaultColor_v.glsl"
			;
		blt::string fSource =
#include "Source\DefaultColor_f.glsl"
			;
		std::unique_ptr<Shader> shader = Shader::FromSource(vSource, fSource);
		Shader* ptr = ResourceManager::Get<Shader>(ResourceManager::Register(std::move(shader))).Get();
		return ptr;
	}

	const Shader* Shader::CreateDefaultTextureShader()
	{
		blt::string vSource =
#include "Source\DefaultTexture_v.glsl"
			;
		blt::string fSource =
#include "Source\DefaultTexture_f.glsl"
			;
		std::unique_ptr<Shader> shader = Shader::FromSource(vSource, fSource);
		Shader* ptr = ResourceManager::Get<Shader>(ResourceManager::Register(std::move(shader))).Get();
		return ptr;
	}

	const Shader* Shader::CreateDefaultFontShader()
	{
		blt::string vSource =
#include "Source\DefaultFont_v.glsl"
			;
		blt::string fSource =
#include "Source\DefaultFont_f.glsl"
			;
		std::unique_ptr<Shader> shader = Shader::FromSource(vSource, fSource);
		Shader* ptr = ResourceManager::Get<Shader>(ResourceManager::Register(std::move(shader))).Get();
		return ptr;
	}

	const Shader* Shader::CreateDefaultSkyboxShader()
	{
		blt::string vSource =
#include "Source\DefaultSkybox_v.glsl"
			;
		blt::string fSource =
#include "Source\DefaultSkybox_f.glsl"
			;
		std::unique_ptr<Shader> shader = Shader::FromSource(vSource, fSource);
		Shader* ptr = ResourceManager::Get<Shader>(ResourceManager::Register(std::move(shader))).Get();
		return ptr;
	}

	const Shader* Shader::CreateSpriteTextureShader()
	{
		blt::string vSource =
#include "Source\SpriteTexture_v.glsl"
			;
		blt::string fSource =
#include "Source\SpriteTexture_f.glsl"
			;
		std::unique_ptr<Shader> shader = Shader::FromSource(vSource, fSource);
		Shader* ptr = ResourceManager::Get<Shader>(ResourceManager::Register(std::move(shader))).Get();
		return ptr;
	}

	const Shader* Shader::CreateLightingColorShader()
	{
		blt::string vSource =
#include "Source\LightingColor_v.glsl"
			;
		blt::string fSource =
#include "Source\LightingColor_f.glsl"
			;
		std::unique_ptr<Shader> shader = Shader::FromSource(vSource, fSource);
		Shader* ptr = ResourceManager::Get<Shader>(ResourceManager::Register(std::move(shader))).Get();
		return ptr;
	}

	const Shader* Shader::CreateLightingTextureShader()
	{
		blt::string vSource =
#include "Source\LightingTexture_v.glsl"
			;
		blt::string fSource =
#include "Source\LightingTexture_f.glsl"
			;
		std::unique_ptr<Shader> shader = Shader::FromSource(vSource, fSource);
		Shader* ptr = ResourceManager::Get<Shader>(ResourceManager::Register(std::move(shader))).Get();
		return ptr;
	}

}