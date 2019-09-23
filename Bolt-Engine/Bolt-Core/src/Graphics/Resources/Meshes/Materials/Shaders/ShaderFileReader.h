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
			blt::string LinkName;
		};

		struct BLT_API UniformInfo
		{
		public:
			blt::string VariableName;
			ValueType Type = ValueType::Void;
			ValueTypeDim Dimension = ValueTypeDim::Single;
			UniformMetadata Meta;
			int Length = 0;
		};

	private:
		blt::string m_VertexSource;
		blt::string m_GeometrySource;
		blt::string m_FragmentSource;

		mutable bool m_IsDirty;
		mutable Ref<ShaderInstance> m_ShaderInstance;

	public:
		ShaderFileReader();
		ShaderFileReader(const Filepath& shaderFile);
		ShaderFileReader(const Filepath& vertexFile, const Filepath& fragmentFile);
		ShaderFileReader(const Filepath& vertexFile, const Filepath& geometryFile, const Filepath& fragmentFile);

		bool HasVertexSource() const;
		bool HasGeometrySource() const;
		bool HasFragmentSource() const;

		const blt::string& GetVertexSource() const;
		const blt::string& GetGeometrySource() const;
		const blt::string& GetFragmentSource() const;

		void SetVertexFile(const Filepath& file);
		void SetGeometryFile(const Filepath& file);
		void SetFragmentFile(const Filepath& file);
		void SetShaderFile(const Filepath& file);

		// Manually set the vertex source to use while building
		void SetVertexSource(const blt::string& source);
		// Manually set the geometry source to use while building
		void SetGeometrySource(const blt::string& source);
		// Manually set the fragment source to use while building
		void SetFragmentSource(const blt::string& source);

		const Ref<ShaderInstance>& GetShader() const;
		ShaderLinkContext GetLinkContext() const;
		void Recompile() const;

	private:
		static void ParseShaderFile(const blt::string& shaderSource, blt::string& outVertexSource, blt::string& outGeometrySource, blt::string& outFragmentSource);
		static int ShaderTypeFromString(const blt::string& str);
		static std::vector<UniformInfo> ProcessShaderSource(blt::string& source);
		static blt::string ReadValue(const blt::string& line, const blt::string& key);
		static blt::string ReadStringValue(const blt::string& line, const blt::string& key);
		static int ReadGlslConstInt(const blt::string& value, const blt::string& source);
		static void PopulateShaderUniforms(CompiledShaderProgram& program, const std::vector<UniformInfo>& uniforms);
	};

}