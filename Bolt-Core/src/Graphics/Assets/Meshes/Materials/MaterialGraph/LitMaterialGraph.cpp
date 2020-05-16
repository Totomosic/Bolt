#include "bltpch.h"
#include "LitMaterialGraph.h"

#include "Nodes/MasterNodes/PBRMasterNodes.h"
#include "Nodes/MasterNodes/LitMasterNodes.h"

#include "Graphics/Assets/AssetManager.h"

namespace Bolt
{

	LitMaterialGraph::LitMaterialGraph(AssetManager* manager) : MaterialGraph(manager),
		m_VertexPosition(nullptr), m_Color(nullptr), m_SpecularColor(nullptr), m_Shininess(nullptr), m_ShineDamper(nullptr), m_Alpha(nullptr), m_AlphaThreshold(nullptr)
	{
		m_VertexPosition = &AddMasterNode("VertexPosition", std::make_unique<VertexPositionNode>());
		m_Color = &AddMasterNode("Color", std::make_unique<AlbedoNode>());
		m_SpecularColor = &AddMasterNode("SpecularColor", std::make_unique<AlbedoNode>());
		m_Shininess = &AddMasterNode("Shininess", std::make_unique<ShininessNode>());
		m_ShineDamper = &AddMasterNode("ShineDamper", std::make_unique<ShineDamperNode>());
		m_Alpha = &AddMasterNode("Alpha", std::make_unique<AlphaNode>());
		m_AlphaThreshold = &AddMasterNode("AlphaThreshold", std::make_unique<AlphaThresholdNode>());
	}

	void LitMaterialGraph::SetVertexPosition(const NodeConnection& connection)
	{
		m_VertexPosition->SetValue(connection);
	}

	void LitMaterialGraph::SetRGB(const NodeConnection& connection)
	{
		m_Color->SetValue(connection);
	}

	void LitMaterialGraph::SetSpecularRGB(const NodeConnection& connection)
	{
		m_SpecularColor->SetValue(connection);
	}

	void LitMaterialGraph::SetShininess(const NodeConnection& connection)
	{
		m_Shininess->SetValue(connection);
	}

	void LitMaterialGraph::SetShineDamper(const NodeConnection& connection)
	{
		m_ShineDamper->SetValue(connection);
	}

	void LitMaterialGraph::SetAlpha(const NodeConnection& connection)
	{
		m_Alpha->SetValue(connection);
	}

	void LitMaterialGraph::SetAlphaThreshold(const NodeConnection& connection)
	{
		m_AlphaThreshold->SetValue(connection);
	}

	void LitMaterialGraph::FinaliseBuild(const std::unordered_map<std::string, ShaderValuePtr>& masterNodeValues)
	{
		// Scale the light intensity to more closely match the PBR lighting
		constexpr float LIGHT_INTENSITY_SCALE = 0.05f;

		VertexShader& vertex = GetBuilder().GetBuilder().Factory().Vertex();
		ShaderVariablePtr modelMatrix = vertex.RendererUniform(RendererUniform::ModelMatrix);
		ShaderVariablePtr viewMatrix = vertex.RendererUniform(RendererUniform::ViewMatrix);
		ShaderVariablePtr projectionMatrix = vertex.RendererUniform(RendererUniform::ProjectionMatrix);
		ShaderVariablePtr normalMatrix = vertex.RendererUniform(RendererUniform::NormalMatrix);
		ShaderVariablePtr cameraPosition = vertex.RendererUniform(RendererUniform::CameraPosition);
		ShaderPassVariablePtr outColor = vertex.DeclarePassOut<Color>();
		ShaderPassVariablePtr outToCamera = vertex.DeclarePassOut<Vector3f>();
		ShaderPassVariablePtr outWorldNormal = vertex.DeclarePassOut<Vector3f>();
		ShaderPassVariablePtr outWorldPos = vertex.DeclarePassOut<Vector3f>();

		ShaderVariablePtr position = vertex.DefineVar(ShaderFuncs::Vec4(masterNodeValues.at("VertexPosition"), ShaderLiteral::FromFloat(1.0f)));
		ShaderVariablePtr worldPos = vertex.DefineVar(ShaderFuncs::Mul(modelMatrix, position));
		ShaderVariablePtr viewPos = vertex.DefineVar(ShaderFuncs::Mul(viewMatrix, worldPos));
		ShaderVariablePtr screenPos = vertex.DefineVar(ShaderFuncs::Mul(projectionMatrix, viewPos));
		vertex.SetVertexPosition(screenPos);
		vertex.SetVariable(outColor, vertex.Stream(BufferLayout::DefaultIndices.Color));
		vertex.SetVariable(outToCamera, ShaderFuncs::Sub(cameraPosition, ShaderFuncs::xyz(worldPos)));
		vertex.SetVariable(outWorldNormal, ShaderFuncs::Mul(ShaderFuncs::Matrix3(normalMatrix), vertex.Stream(BufferLayout::DefaultIndices.Normal)));
		vertex.SetVariable(outWorldPos, ShaderFuncs::xyz(worldPos));

		FragmentShader& fragment = GetBuilder().GetBuilder().Factory().Fragment();
		ShaderPassVariablePtr inColor = fragment.DeclarePassIn(outColor);
		ShaderPassVariablePtr inToCamera = fragment.DeclarePassIn(outToCamera);
		ShaderPassVariablePtr inWorldNormal = fragment.DeclarePassIn(outWorldNormal);
		ShaderPassVariablePtr inWorldPos = fragment.DeclarePassIn(outWorldPos);
		ShaderVariablePtr lightPositions = fragment.RendererUniform(RendererUniform::LightPositions);
		ShaderVariablePtr lightColors = fragment.RendererUniform(RendererUniform::LightColors);
		ShaderVariablePtr lightAmbients = fragment.RendererUniform(RendererUniform::LightAmbients);
		ShaderVariablePtr lightAmbientColors = fragment.RendererUniform(RendererUniform::LightAmbientColors);
		ShaderVariablePtr lightIntensities = fragment.RendererUniform(RendererUniform::LightIntensities);
		ShaderVariablePtr lightCount = fragment.RendererUniform(RendererUniform::LightCount);

		IfScope& alphaThresholdTest = fragment.If(ShaderFuncs::LessThan(masterNodeValues.at("Alpha"), masterNodeValues.at("AlphaThreshold")));
		alphaThresholdTest.Discard();
		ShaderVariablePtr color = fragment.DefineVar(masterNodeValues.at("Color"));
		ShaderVariablePtr specularColor = fragment.DefineVar(masterNodeValues.at("SpecularColor"));
		ShaderVariablePtr reflectivity = fragment.DefineVar(masterNodeValues.at("Shininess"));
		ShaderVariablePtr shineDamper = fragment.DefineVar(masterNodeValues.at("ShineDamper"));

		ShaderVariablePtr totalDiffuse = fragment.DefineVar(ShaderLiteral::FromVec3({ 0.0f, 0.0f, 0.0f }));
		ShaderVariablePtr totalSpecular = fragment.DefineVar(ShaderLiteral::FromVec3({ 0.0f, 0.0f, 0.0f }));
		ShaderVariablePtr totalAmbient = fragment.DefineVar(ShaderLiteral::FromVec3({ 0.0f, 0.0f, 0.0f }));
		ShaderVariablePtr counter = fragment.DeclareVar<int>();
		ForLoopScope& loop = fragment.For(counter, ShaderLiteral::FromInt(0), ShaderFuncs::LessThan(counter, lightCount), std::make_unique<AddAssignOp>(counter, ShaderLiteral::FromInt(1)));
		ShaderVariablePtr unitToLightVec = loop.DefineVar(ShaderFuncs::Normalize(ShaderFuncs::Sub(ShaderFuncs::Index(lightPositions, counter), inWorldPos)));
		ShaderVariablePtr unitToCamVec = loop.DefineVar(ShaderFuncs::Normalize(inToCamera));
		ShaderVariablePtr unitNormal = loop.DefineVar(ShaderFuncs::Normalize(inWorldNormal));
		ShaderVariablePtr reflectedLight = loop.DefineVar(ShaderFuncs::Reflect(ShaderFuncs::Neg(unitToLightVec), unitNormal));
		ShaderVariablePtr nDotl = loop.DefineVar(ShaderFuncs::Dot(unitNormal, unitToLightVec));
		ShaderVariablePtr multiplier = loop.DefineVar(ShaderFuncs::Max(nDotl, ShaderLiteral::FromFloat(0.0f)));
		ShaderVariablePtr specularFactor = loop.DefineVar(ShaderFuncs::Max(ShaderFuncs::Dot(reflectedLight, unitToCamVec), ShaderLiteral::FromFloat(0.0f)));
		ShaderVariablePtr dampedSpecFactor = loop.DefineVar(ShaderFuncs::Pow(specularFactor, shineDamper));
		ShaderVariablePtr effectiveIntensity = loop.DefineVar(ShaderFuncs::Mul(ShaderFuncs::Index(lightIntensities, counter), ShaderLiteral::FromFloat(LIGHT_INTENSITY_SCALE)));
		ShaderVariablePtr lightColor = loop.DefineVar(ShaderFuncs::Mul(ShaderFuncs::Index(lightColors, counter), effectiveIntensity));
		ShaderVariablePtr ambientColor = loop.DefineVar(ShaderFuncs::Mul(ShaderFuncs::Index(lightAmbientColors, counter), ShaderFuncs::Index(lightAmbients, counter)));
		loop.AddAssign(totalDiffuse, ShaderFuncs::xyz(ShaderFuncs::Mul(lightColor, multiplier)));
		loop.AddAssign(totalSpecular, ShaderFuncs::xyz(ShaderFuncs::Mul(ShaderFuncs::Mul(lightColor, dampedSpecFactor), reflectivity)));
		loop.AddAssign(totalAmbient, ShaderFuncs::xyz(ambientColor));
		ShaderVariablePtr finalColor = fragment.DefineVar(ShaderFuncs::Add(ShaderFuncs::Mul(ShaderFuncs::Add(totalDiffuse, totalAmbient), color), ShaderFuncs::Mul(totalSpecular, specularColor)));
		fragment.SetFragColor(ShaderFuncs::Vec4(finalColor, masterNodeValues.at("Alpha")));
	}

}