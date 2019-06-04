#include "bltpch.h"
#include "SampleTextureNode.h"
#include "../MaterialGraph.h"
#include "Graphics/Resources/ResourceManager.h"

namespace Bolt
{

	SampleTextureNode::SampleTextureNode(SampleMode mode) : MaterialNode(ShaderStageCompatibility::FragmentOnly, 2, 5),
		m_Mode(mode)
	{
		InputPort texturePort(ValueType::Texture2D);
		InputPort uvPort(ValueType::Vector2f);
		OutputPort outColorPort(ValueType::Vector4f);
		OutputPort outRPort(ValueType::Float);
		OutputPort outGPort(ValueType::Float);
		OutputPort outBPort(ValueType::Float);
		OutputPort outAPort(ValueType::Float);
		SetInput(0, texturePort);
		SetInput(1, uvPort);
		SetOutput(0, outColorPort);
		SetOutput(1, outRPort);
		SetOutput(2, outGPort);
		SetOutput(3, outBPort);
		SetOutput(4, outAPort);
	}

	void SampleTextureNode::SetTexture(const NodeConnection& connection)
	{
		Connect(0, connection);
	}

	void SampleTextureNode::SetTexCoord(const NodeConnection& connection)
	{
		Connect(1, connection);
	}

	NodeConnection SampleTextureNode::GetColor() const
	{
		return GetConnection(0);
	}

	NodeConnection SampleTextureNode::GetR() const
	{
		return GetConnection(1);
	}

	NodeConnection SampleTextureNode::GetG() const
	{
		return GetConnection(2);
	}

	NodeConnection SampleTextureNode::GetB() const
	{
		return GetConnection(3);
	}

	NodeConnection SampleTextureNode::GetA() const
	{
		return GetConnection(4);
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
		node.BuildOutput(1, ShaderFuncs::x(color));
		node.BuildOutput(2, ShaderFuncs::y(color));
		node.BuildOutput(3, ShaderFuncs::z(color));
		node.BuildOutput(4, ShaderFuncs::w(color));
	}

	void SampleTextureNode::ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context)
	{
		MaterialNode& texture = graph.AddProperty(ShaderProgram::NAMELESS_UNIFORM, ResourceManager::Get().Textures().DefaultWhite());
		Connect(0, texture.GetConnection(0));
		Connect(1, context.VertexTexCoord().GetConnection(0));
	}

}