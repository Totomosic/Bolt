#include "bltpch.h"
#include "SampleTextureNode.h"
#include "../MaterialGraph.h"
#include "Graphics/Resources/ResourceManager.h"

namespace Bolt
{

	SampleTextureNode::SampleTextureNode(SampleMode mode) : MaterialNode(ShaderStageCompatibility::FragmentOnly, 2, 6),
		m_Mode(mode)
	{
		InputPort texturePort(ValueType::Texture2D);
		InputPort uvPort(ValueType::Vector2f);
		OutputPort outRGBAPort(ValueType::Vector4f);
		OutputPort outRGBPort(ValueType::Vector3f);
		OutputPort outRPort(ValueType::Float);
		OutputPort outGPort(ValueType::Float);
		OutputPort outBPort(ValueType::Float);
		OutputPort outAPort(ValueType::Float);
		SetInput(0, texturePort);
		SetInput(1, uvPort);
		SetOutput(0, outRGBAPort);
		SetOutput(1, outRGBPort);
		SetOutput(2, outRPort);
		SetOutput(3, outGPort);
		SetOutput(4, outBPort);
		SetOutput(5, outAPort);
	}

	SampleMode SampleTextureNode::GetSampleMode() const
	{
		return m_Mode;
	}

	void SampleTextureNode::SetSampleMode(SampleMode mode)
	{
		m_Mode = mode;
	}

	void SampleTextureNode::SetTexture(const NodeConnection& connection)
	{
		Connect(0, connection);
	}

	void SampleTextureNode::SetTexCoord(const NodeConnection& connection)
	{
		Connect(1, connection);
	}

	NodeConnection SampleTextureNode::GetRGBA() const
	{
		return GetConnection(0);
	}

	NodeConnection SampleTextureNode::GetRGB() const
	{
		return GetConnection(1);
	}

	NodeConnection SampleTextureNode::GetR() const
	{
		return GetConnection(2);
	}

	NodeConnection SampleTextureNode::GetG() const
	{
		return GetConnection(3);
	}

	NodeConnection SampleTextureNode::GetB() const
	{
		return GetConnection(4);
	}

	NodeConnection SampleTextureNode::GetA() const
	{
		return GetConnection(5);
	}

	void SampleTextureNode::Build(BuiltMaterialNode& node, const LinkedInputs& inputs, const MaterialGraphContext& context, MaterialGraphBuilder& builder) const
	{
		ShaderProgram& program = builder.GetProgram();
		ShaderVariablePtr color;
		switch (m_Mode)
		{
		case SampleMode::Color:
			color = program.DefineVar(ShaderFuncs::Pow(ShaderFuncs::SampleTexture(inputs.GetInput(0), inputs.GetInput(1)), ShaderLiteral::FromVec4({ 2.2f, 2.2f, 2.2f, 1.0f })));
			break;
		case SampleMode::Normal:
			color = program.DefineVar(ShaderFuncs::SampleTexture(inputs.GetInput(0), inputs.GetInput(1)));
			break;
		}
		node.BuildOutput(0, color);
		node.BuildOutput(1, ShaderFuncs::xyz(color));
		node.BuildOutput(2, ShaderFuncs::x(color));
		node.BuildOutput(3, ShaderFuncs::y(color));
		node.BuildOutput(4, ShaderFuncs::z(color));
		node.BuildOutput(5, ShaderFuncs::w(color));
	}

	void SampleTextureNode::ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context)
	{
		PropertyNode& texture = graph.AddProperty(ShaderProgram::NAMELESS_UNIFORM, graph.GetResourceManager().Textures().DefaultWhite());
		Connect(0, texture.GetValue());
		Connect(1, context.VertexTexCoord().GetValue());
	}

}