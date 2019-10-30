#include "bltpch.h"
#include "BasicMaterialGraph.h"

#include "Nodes/MasterNodes/PBRMasterNodes.h"
#include "Nodes/MasterNodes/LitMasterNodes.h"

#include "Graphics/Resources/ResourceManager.h"

namespace Bolt
{

	BasicMaterialGraph::BasicMaterialGraph(ResourceManager* manager) : MaterialGraph(manager),
		m_VertexPosition(nullptr), m_Color(nullptr), m_Alpha(nullptr), m_AlphaThreshold(nullptr)
	{
		m_VertexPosition = &AddMasterNode("VertexPosition", std::make_unique<VertexPositionNode>());
		m_Color = &AddMasterNode("Color", std::make_unique<AlbedoNode>());
		m_Alpha = &AddMasterNode("Alpha", std::make_unique<AlphaNode>());
		m_AlphaThreshold = &AddMasterNode("AlphaThreshold", std::make_unique<AlphaThresholdNode>());
	}

	void BasicMaterialGraph::SetVertexPosition(const NodeConnection& connection)
	{
		m_VertexPosition->SetValue(connection);
	}

	void BasicMaterialGraph::SetRGB(const NodeConnection& connection)
	{
		m_Color->SetValue(connection);
	}

	void BasicMaterialGraph::SetAlpha(const NodeConnection& connection)
	{
		m_Alpha->SetValue(connection);
	}

	void BasicMaterialGraph::SetAlphaThreshold(const NodeConnection& connection)
	{
		m_AlphaThreshold->SetValue(connection);
	}

	void BasicMaterialGraph::FinaliseBuild(const std::unordered_map<blt::string, ShaderValuePtr>& masterNodeValues)
	{
		VertexShader& vertex = GetBuilder().GetBuilder().Factory().Vertex();
		ShaderVariablePtr modelMatrix = vertex.RendererUniform(RendererUniform::ModelMatrix);
		ShaderVariablePtr viewMatrix = vertex.RendererUniform(RendererUniform::ViewMatrix);
		ShaderVariablePtr projectionMatrix = vertex.RendererUniform(RendererUniform::ProjectionMatrix);
		ShaderPassVariablePtr outColor = vertex.DeclarePassOut<Color>();

		ShaderVariablePtr position = vertex.DefineVar(ShaderFuncs::Vec4(masterNodeValues.at("VertexPosition"), ShaderLiteral::FromFloat(1.0f)));
		ShaderVariablePtr worldPos = vertex.DefineVar(ShaderFuncs::Mul(modelMatrix, position));
		ShaderVariablePtr viewPos = vertex.DefineVar(ShaderFuncs::Mul(viewMatrix, worldPos));
		ShaderVariablePtr screenPos = vertex.DefineVar(ShaderFuncs::Mul(projectionMatrix, viewPos));
		vertex.SetVertexPosition(screenPos);
		vertex.SetVariable(outColor, vertex.Stream(BufferLayout::COLOR_INDEX));

		FragmentShader& fragment = GetBuilder().GetBuilder().Factory().Fragment();
		ShaderPassVariablePtr inColor = fragment.DeclarePassIn(outColor);

		IfScope& alphaThresholdTest = fragment.If(ShaderFuncs::LessThan(masterNodeValues.at("Alpha"), masterNodeValues.at("AlphaThreshold")));
		alphaThresholdTest.Discard();
		ShaderVariablePtr color = fragment.DefineVar(masterNodeValues.at("Color"));
		ShaderVariablePtr finalColor = fragment.DefineVar(ShaderFuncs::Mul(ShaderFuncs::xyz(inColor), color));
		fragment.SetVariable(finalColor, ShaderFuncs::Pow(finalColor, ShaderLiteral::FromVec3({ 1 / 2.2f, 1 / 2.2f, 1 / 2.2f })));
		fragment.SetFragColor(ShaderFuncs::Vec4(finalColor, masterNodeValues.at("Alpha")));
	}

}