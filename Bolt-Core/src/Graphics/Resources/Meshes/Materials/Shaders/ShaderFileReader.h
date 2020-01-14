#pragma once
#include "ShaderLinkContext.h"

namespace Bolt
{

	class BLT_API ShaderFileReader
	{
	private:
		struct BLT_API UniformMetadata
		{
		public:
			bool IsRenderer = false;
			RendererUniform Uniform;
			std::string LinkName;
		};

		struct BLT_API UniformInfo
		{
		public:
			std::string VariableName;
			ValueType Type = ValueType::Void;
			ValueTypeDim Dimension = ValueTypeDim::Single;
			UniformMetadata Meta;
			int Length = 0;
		};

	private:
		std::string m_VertexSource;
		std::string m_GeometrySource;
		std::string m_FragmentSource;

		mutable bool m_IsDirty;
		mutable Ref<ShaderInstance> m_ShaderInstance;

	public:
		ShaderFileReader();
		ShaderFileReader(const FilePath& shaderFile);
		ShaderFileReader(const FilePath& vertexFile, const FilePath& fragmentFile);
		ShaderFileReader(const FilePath& vertexFile, const FilePath& geometryFile, const FilePath& fragmentFile);

		bool HasVertexSource() const;
		bool HasGeometrySource() const;
		bool HasFragmentSource() const;

		const std::string& GetVertexSource() const;
		const std::string& GetGeometrySource() const;
		const std::string& GetFragmentSource() const;

		void SetVertexFile(const FilePath& file);
		void SetGeometryFile(const FilePath& file);
		void SetFragmentFile(const FilePath& file);
		void SetShaderFile(const FilePath& file);

		// Manually set the vertex source to use while building
		void SetVertexSource(const std::string& source);
		// Manually set the geometry source to use while building
		void SetGeometrySource(const std::string& source);
		// Manually set the fragment source to use while building
		void SetFragmentSource(const std::string& source);

		const Ref<ShaderInstance>& GetShader() const;
		ShaderLinkContext GetLinkContext() const;
		void Recompile() const;

	private:
		static void ParseShaderFile(const std::string& shaderSource, std::string& outVertexSource, std::string& outGeometrySource, std::string& outFragmentSource);
		static int ShaderTypeFromString(const std::string& str);
		static std::vector<UniformInfo> ProcessShaderSource(std::string& source);
		static std::string ReadValue(const std::string& line, const std::string& key);
		static std::string ReadStringValue(const std::string& line, const std::string& key);
		static int ReadGlslConstInt(const std::string& value, const std::string& source);
		static void PopulateShaderUniforms(CompiledShaderProgram& program, const std::vector<UniformInfo>& uniforms);
	};

}