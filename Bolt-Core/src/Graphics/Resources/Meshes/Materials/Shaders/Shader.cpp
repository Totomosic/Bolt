#include "bltpch.h"

#include "Shader.h"
#include "Graphics/Resources/ResourceManager.h"
#include "../Material.h"
#include "Engine/Renderer/GLState.h"

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

	Shader::Shader(const std::string& vertexSource, const std::string& fragmentSource) : Resource(),
		m_Id(0)
	{
		Create();
		id_t vertex = AddShader(vertexSource, GL_VERTEX_SHADER);
		id_t fragment = AddShader(fragmentSource, GL_FRAGMENT_SHADER);
		id_t arr[2] = { vertex, fragment };
		Finalise(arr, 2);
	}

	Shader::Shader(const std::string& vertexSource, const std::string& geometrySource, const std::string& fragmentSource) : Resource(),
		m_Id(0)
	{
		Create();
		id_t vertex = AddShader(vertexSource, GL_VERTEX_SHADER);
		id_t geometry = AddShader(geometrySource, GL_GEOMETRY_SHADER);
		id_t fragment = AddShader(fragmentSource, GL_FRAGMENT_SHADER);
		id_t arr[3] = { vertex, geometry, fragment };
		Finalise(arr, 3);
	}

	Shader::Shader(Shader&& other) noexcept
		: m_Id(other.m_Id)
	{
		other.m_Id = 0;
	}

	Shader& Shader::operator=(Shader&& other) noexcept
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

	void Shader::SetUniform(int location, bool value) const
	{	
		GL_CALL(glUniform1i(location, value));
	}

	void Shader::SetUniform(int location, int value) const
	{
		GL_CALL(glUniform1i(location, value));
	}

	void Shader::SetUniform(int location, uint32_t value) const
	{
		GL_CALL(glUniform1i(location, (int)value));
	}

	void Shader::SetUniform(int location, float value) const
	{		
		GL_CALL(glUniform1f(location, value));
	}

	void Shader::SetUniform(int location, double value) const
	{
		GL_CALL(glUniform1d(location, value));
	}

	void Shader::SetUniform(int location, const Vector2f& value) const
	{
		GL_CALL(glUniform2f(location, value.x, value.y));
	}

	void Shader::SetUniform(int location, const Vector2i& value) const
	{
		GL_CALL(glUniform2i(location, value.x, value.y));
	}

	void Shader::SetUniform(int location, const Vector3f& value) const
	{
		GL_CALL(glUniform3f(location, value.x, value.y, value.z));
	}

	void Shader::SetUniform(int location, const Vector3i& value) const
	{
		GL_CALL(glUniform3i(location, value.x, value.y, value.z));
	}

	void Shader::SetUniform(int location, const Vector4f& value) const
	{
		GL_CALL(glUniform4f(location, value.x, value.y, value.z, value.w));
	}

	void Shader::SetUniform(int location, const Vector4i& value) const
	{
		GL_CALL(glUniform4i(location, value.x, value.y, value.z, value.w));
	}

	void Shader::SetUniform(int location, const Color& value) const
	{
		GL_CALL(glUniform4f(location, value.r, value.g, value.b, value.a));
	}

	void Shader::SetUniform(int location, const Matrix2f& value) const
	{
		GL_CALL(glUniformMatrix2fv(location, 1, GL_FALSE, value.values));
	}

	void Shader::SetUniform(int location, const Matrix3f& value) const
	{
		GL_CALL(glUniformMatrix3fv(location, 1, GL_FALSE, value.values));
	}

	void Shader::SetUniform(int location, const Matrix4f& value) const
	{
		GL_CALL(glUniformMatrix4fv(location, 1, GL_FALSE, value.values));
	}

	void Shader::SetUniform(int location, const Quaternion& value) const
	{
		GL_CALL(glUniform4f(location, value.x, value.y, value.z, value.w));
	}

	std::unique_ptr<Resource> Shader::Clone() const
	{
		BLT_ASSERT(false, "Currently unable to clone Shaders");
		return nullptr;
	}

	std::unique_ptr<Shader> Shader::FromFile(const FilePath& shaderFile)
	{
		File f = Filesystem::Open(shaderFile, OpenMode::Read);
		std::string fileData = f.ReadText();
		return FromSource(fileData);
	}

	std::unique_ptr<Shader> Shader::FromFile(const FilePath& vertexFile, const FilePath& fragmentFile)
	{
		File v = Filesystem::Open(vertexFile, OpenMode::Read);
		File f = Filesystem::Open(fragmentFile, OpenMode::Read);
		return FromSource(v.ReadText(), f.ReadText());
	}

	std::unique_ptr<Shader> Shader::FromFile(const FilePath& vertexFile, const FilePath& geometryFile, const FilePath& fragmentFile)
	{
		File v = Filesystem::Open(vertexFile, OpenMode::Read);
		File g = Filesystem::Open(geometryFile, OpenMode::Read);
		File f = Filesystem::Open(fragmentFile, OpenMode::Read);
		return FromSource(v.ReadText(), g.ReadText(), f.ReadText());
	}

	std::unique_ptr<Shader> Shader::FromSource(const std::string& source)
	{
		constexpr int NONE_SHADER_TYPE = -1;
		constexpr int VERTEX_SHADER_TYPE = 0;
		constexpr int FRAGMENT_SHADER_TYPE = 2;
		constexpr int GEOMETRY_SHADER_TYPE = 1;

		std::stringstream ss[3];
		int currentType = NONE_SHADER_TYPE;
		bool isGeometryShader = false;

		std::string fileData = source;
		std::vector<std::string_view> lines = blt::split_view(fileData, '\n');
		for (std::string_view& line : lines)
		{
			if (line.find("#shader") != std::string::npos)
			{
				if (line.find("vertex") != std::string::npos)
				{
					currentType = VERTEX_SHADER_TYPE;
				}
				else if (line.find("fragment") != std::string::npos)
				{
					currentType = FRAGMENT_SHADER_TYPE;
				}
				else if (line.find("geometry") != std::string::npos)
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
		std::string sources[3] = { ss[0].str(), ss[1].str(), ss[2].str() };
		return (isGeometryShader) ? FromSource(sources[0], sources[1], sources[2]) : FromSource(sources[0], sources[2]);
	}

	std::unique_ptr<Shader> Shader::FromSource(const std::string& vertexSource, const std::string& fragmentSource)
	{
		return std::make_unique<Shader>(vertexSource, fragmentSource);
	}

	std::unique_ptr<Shader> Shader::FromSource(const std::string& vertexSource, const std::string& geometrySource, const std::string& fragmentSource)
	{
		return std::make_unique<Shader>(vertexSource, geometrySource, fragmentSource);
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
			std::string errorMsg(&error[0]);
			BLT_CORE_ERROR("Shader Failed To Link");
			BLT_CORE_ERROR(errorMsg);
		}
		for (int i = 0; i < count; i++)
		{
			GL_CALL(glDetachShader(m_Id, shaders[i]));
			GL_CALL(glDeleteShader(shaders[i]));
		}
	}

	id_t Shader::AddShader(const std::string& source, GLenum shaderType)
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
			std::string errorMessage(&error[0]);
			GL_CALL(glDeleteShader(shader));
			BLT_CORE_ERROR(errorMessage);
			return 0;
		}
		GL_CALL(glAttachShader(m_Id, shader));
		return shader;
	}

	int Shader::GetUniformLocation(const std::string& location) const
	{
		Bind();
		int loc = GL_CALL(glGetUniformLocation(m_Id, location.c_str()));
		return loc;
	}

}