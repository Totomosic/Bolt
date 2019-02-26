#pragma once
#include "Components\__Components__.h"

namespace Bolt
{

	class BLT_API ShaderProgramFactory
	{
	public:
		struct BLT_API StreamValueInfo
		{
		public:
			ShaderStream Stream;
			ShaderValuePtr Value;
		};

		struct BLT_API ShaderAttributeInfo
		{
		public:
			ShaderAttribute Attribute;
			ShaderValuePtr Value;
		};

		struct BLT_API ShaderPassInfo
		{
		public:
			ShaderValuePtr Value;
			ShaderValuePtr PassedValue;
		};

		struct BLT_API ShaderValueInfo
		{
		public:
			ShaderValuePtr Value;
			blt::string AssignmentString;
		};

	private:
		ShaderType m_CurrentShader;
		FunctionContext m_Context;
		std::vector<StreamValueInfo> m_StreamValues;
		std::vector<ShaderValuePtr> m_Uniforms;
		std::vector<ShaderPassInfo> m_PassValues;
		std::vector<ShaderValuePtr> m_PassedToMe;
		std::vector<ShaderAttributeInfo> m_Attributes;
		std::vector<ShaderValueInfo> m_Values;		

	public:
		ShaderProgramFactory(ShaderType shader);

		const FunctionContext& Context() const;
		FunctionContext& Context();
		ValueType GetValueTypeOfStream(ShaderStream stream) const;
		ShaderType GetCurrentShader() const;

		ShaderValuePtr GetStream(ShaderStream stream);
		ShaderValuePtr RendererUniform(Bolt::RendererUniform uniform);
		ShaderValuePtr Uniform(ValueType type);
		ShaderValuePtr Pass(ShaderValuePtr value);
		ShaderValuePtr FuncResult(const FunctionDecl& declaration, const std::vector<ShaderValuePtr>& inputs);
		ShaderValuePtr FuncResult(const BuiltInFunc& declaration, const std::vector<ShaderValuePtr>& inputs);

		void SetAttribute(ShaderAttribute attrib, ShaderValuePtr value);		

		blt::string CreateSource() const;

		friend class ShaderFactory;
		friend class ShaderInstance;

	private:
		static blt::string GetStreamName(ShaderStream stream);
		static int IndexOfStream(ShaderStream stream);
		static blt::string GetAttributeName(ShaderAttribute attrib);
		static ValueType GetAttributeType(ShaderAttribute attrib);
		static blt::string GetRendererUniformName(Bolt::RendererUniform uniform);
		static ValueType GetRendererUniformType(Bolt::RendererUniform uniform);

		void AddValuePassedToMe(ShaderValuePtr value);

		blt::string GetShaderName() const;
		blt::string GetNextUniformName() const;
		blt::string GetNextPassName() const;
		blt::string GetNextValueName() const;

	};

}