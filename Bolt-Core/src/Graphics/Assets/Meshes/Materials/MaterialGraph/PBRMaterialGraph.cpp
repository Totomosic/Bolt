#include "bltpch.h"
#include "PBRMaterialGraph.h"

#include "Nodes/MasterNodes/PBRMasterNodes.h"
#include "Graphics/Assets/AssetManager.h"

namespace Bolt
{

	PBRMaterialGraph::PBRMaterialGraph(AssetManager* manager) : MaterialGraph(manager),
		m_VertexPosition(nullptr), m_Albedo(nullptr), m_Normal(nullptr), m_Metallic(nullptr), m_Roughness(nullptr), m_Occlusion(nullptr), m_Alpha(nullptr), m_AlphaThreshold(nullptr)
	{
		m_VertexPosition = &AddMasterNode("VertexPosition", std::make_unique<VertexPositionNode>());
		m_Albedo = &AddMasterNode("Albedo", std::make_unique<AlbedoNode>());
		m_Normal = &AddMasterNode("Normal", std::make_unique<NormalNode>());
		m_Metallic = &AddMasterNode("Metallic", std::make_unique<MetallicNode>());
		m_Roughness = &AddMasterNode("Roughness", std::make_unique<RoughnessNode>());
		m_Occlusion = &AddMasterNode("Occlusion", std::make_unique<OcclusionNode>());
		m_Alpha = &AddMasterNode("Alpha", std::make_unique<AlphaNode>());
		m_AlphaThreshold = &AddMasterNode("AlphaThreshold", std::make_unique<AlphaThresholdNode>());
	}

	void PBRMaterialGraph::SetVertexPosition(const NodeConnection& connection)
	{
		m_VertexPosition->SetValue(connection);
	}

	void PBRMaterialGraph::SetRGB(const NodeConnection& connection)
	{
		m_Albedo->SetValue(connection);
	}

	void PBRMaterialGraph::SetNormal(const NodeConnection& connection)
	{
		m_Normal->SetValue(connection);
	}

	void PBRMaterialGraph::SetMetallic(const NodeConnection& connection)
	{
		m_Metallic->SetValue(connection);
	}

	void PBRMaterialGraph::SetRoughness(const NodeConnection& connection)
	{
		m_Roughness->SetValue(connection);
	}

	void PBRMaterialGraph::SetOcclusion(const NodeConnection& connection)
	{
		m_Occlusion->SetValue(connection);
	}

	void PBRMaterialGraph::SetAlpha(const NodeConnection& connection)
	{
		m_Alpha->SetValue(connection);
	}

	void PBRMaterialGraph::SetAlphaThreshold(const NodeConnection& connection)
	{
		m_AlphaThreshold->SetValue(connection);
	}

	void PBRMaterialGraph::FinaliseBuild(const std::unordered_map<std::string, ShaderValuePtr>& masterNodeValues)
	{
		VertexShader& vertex = GetBuilder().GetBuilder().Factory().Vertex();
		ShaderVariablePtr modelMatrix = vertex.RendererUniform(RendererUniform::ModelMatrix);
		ShaderVariablePtr viewMatrix = vertex.RendererUniform(RendererUniform::ViewMatrix);
		ShaderVariablePtr projectionMatrix = vertex.RendererUniform(RendererUniform::ProjectionMatrix);
		ShaderVariablePtr normalMatrix = vertex.RendererUniform(RendererUniform::NormalMatrix);
		ShaderPassVariablePtr outWorldPos = vertex.DeclarePassOut(ValueType::Vector3f);
		ShaderPassVariablePtr outWorldNormal = vertex.DeclarePassOut(ValueType::Vector3f);
		ShaderPassVariablePtr outTBNMatrix = vertex.DeclarePassOut(ValueType::Matrix3f);

		ShaderVariablePtr position = vertex.DefineVar(ShaderFuncs::Vec4(masterNodeValues.at("VertexPosition"), ShaderLiteral::FromFloat(1.0f)));
		ShaderVariablePtr worldPosition = vertex.DefineVar(ShaderFuncs::Mul(modelMatrix, position));
		ShaderVariablePtr viewPosition = vertex.DefineVar(ShaderFuncs::Mul(viewMatrix, worldPosition));
		ShaderVariablePtr screenPosition = vertex.DefineVar(ShaderFuncs::Mul(projectionMatrix, viewPosition));

		ShaderVariablePtr worldNormal = vertex.DefineVar(ShaderFuncs::Mul(ShaderFuncs::Matrix3(normalMatrix), vertex.Stream(BufferLayout::DefaultIndices.Normal)));

		ShaderVariablePtr tangent = vertex.DefineVar(ShaderFuncs::xyz(ShaderFuncs::Mul(modelMatrix, ShaderFuncs::Vec4(vertex.Stream(BufferLayout::DefaultIndices.Tangent), ShaderLiteral::FromFloat(0.0f)))));
		ShaderVariablePtr bitangent = vertex.DefineVar(ShaderFuncs::Cross(worldNormal, tangent));
		// We want a matrix that transforms from tangent space to world space
		vertex.SetVariable(outTBNMatrix, ShaderFuncs::Matrix3(tangent, bitangent, worldNormal));

		vertex.SetVertexPosition(screenPosition);
		vertex.SetVariable(outWorldPos, ShaderFuncs::xyz(worldPosition));
		vertex.SetVariable(outWorldNormal, worldNormal);

		FragmentShader& fragment = GetBuilder().GetBuilder().Factory().Fragment();
		ShaderPassVariablePtr inWorldPos = fragment.DeclarePassIn(outWorldPos);
		ShaderPassVariablePtr inWorldNormal = fragment.DeclarePassIn(outWorldNormal);
		ShaderPassVariablePtr inTBNMatrix = fragment.DeclarePassIn(outTBNMatrix);
		ShaderVariablePtr cameraPosition = fragment.RendererUniform(RendererUniform::CameraPosition);
		ShaderVariablePtr lightPositions = fragment.RendererUniform(RendererUniform::LightPositions);
		ShaderVariablePtr lightColors = fragment.RendererUniform(RendererUniform::LightColors);
		ShaderVariablePtr lightAmbients = fragment.RendererUniform(RendererUniform::LightAmbients);
		ShaderVariablePtr lightAmbientColors = fragment.RendererUniform(RendererUniform::LightAmbientColors);
		ShaderVariablePtr lightIntensities = fragment.RendererUniform(RendererUniform::LightIntensities);
		ShaderVariablePtr lightAttenuations = fragment.RendererUniform(RendererUniform::LightAttenuations);
		ShaderVariablePtr lightCount = fragment.RendererUniform(RendererUniform::LightCount);

		// Takes a normal in tangent space ((0, 0, 1) is default normal) and transforms it into world space
		FunctionScope& calcWorldNormal = fragment.DefineFunction<Vector3f, Vector3f>("calcWorldNormal");
		calcWorldNormal.Return(ShaderFuncs::Normalize(ShaderFuncs::Mul(inTBNMatrix, calcWorldNormal.GetArgument(0))));

		FunctionScope& fresnelSchlick = fragment.DefineFunction<Vector3f, float, Vector3f>("fresnelSchlick");
		ShaderVariablePtr oneTakeF = fresnelSchlick.DefineVar(ShaderFuncs::Sub(ShaderLiteral::FromFloat(1.0f), fresnelSchlick.GetArgument(1)));
		ShaderVariablePtr oneTakeCosTheta = fresnelSchlick.DefineVar(ShaderFuncs::Sub(ShaderLiteral::FromFloat(1.0f), fresnelSchlick.GetArgument(0)));
		fresnelSchlick.Return(ShaderFuncs::Add(fresnelSchlick.GetArgument(1), ShaderFuncs::Mul(oneTakeF, ShaderFuncs::Pow(oneTakeCosTheta, ShaderLiteral::FromFloat(5.0f)))));

		FunctionScope& distributionGGX = fragment.DefineFunction<float, Vector3f, Vector3f, float>("DistributionGGX");
		ShaderVariablePtr a = distributionGGX.DefineVar(ShaderFuncs::Mul(distributionGGX.GetArgument(2), distributionGGX.GetArgument(2)));
		ShaderVariablePtr a2 = distributionGGX.DefineVar(ShaderFuncs::Mul(a, a));
		ShaderVariablePtr nDotH = distributionGGX.DefineVar(ShaderFuncs::Max(ShaderFuncs::Dot(distributionGGX.GetArgument(0), distributionGGX.GetArgument(1)), ShaderLiteral::FromFloat(0.0f)));
		ShaderVariablePtr nDotH2 = distributionGGX.DefineVar(ShaderFuncs::Mul(nDotH, nDotH));
		ShaderVariablePtr dNum = distributionGGX.DefineVar(a2);
		ShaderVariablePtr dDenom = distributionGGX.DefineVar(ShaderFuncs::Add(ShaderFuncs::Mul(nDotH2, ShaderFuncs::Sub(a2, ShaderLiteral::FromFloat(1.0f))), ShaderLiteral::FromFloat(1.0f)));
		distributionGGX.SetVariable(dDenom, ShaderFuncs::Mul(ShaderLiteral::Pi(), ShaderFuncs::Mul(dDenom, dDenom)));
		distributionGGX.Return(ShaderFuncs::Div(dNum, dDenom));

		FunctionScope& geometrySchlickGGX = fragment.DefineFunction<float, float, float>("GeometrySchlickGGX");
		ShaderVariablePtr r = geometrySchlickGGX.DefineVar(ShaderFuncs::Add(geometrySchlickGGX.GetArgument(1), ShaderLiteral::FromFloat(1.0f)));
		ShaderVariablePtr k = geometrySchlickGGX.DefineVar(ShaderFuncs::Div(ShaderFuncs::Mul(r, r), ShaderLiteral::FromFloat(8.0f)));
		ShaderVariablePtr gNum = geometrySchlickGGX.DefineVar(geometrySchlickGGX.GetArgument(0));
		ShaderVariablePtr gDenom = geometrySchlickGGX.DefineVar(ShaderFuncs::Add(ShaderFuncs::Mul(gNum, ShaderFuncs::Sub(ShaderLiteral::FromFloat(1.0f), k)), k));
		geometrySchlickGGX.Return(ShaderFuncs::Div(gNum, gDenom));

		FunctionScope& geometrySmith = fragment.DefineFunction<float, Vector3f, Vector3f, Vector3f, float>("GeometrySmith");
		ShaderVariablePtr gNdotV = geometrySmith.DefineVar(ShaderFuncs::Max(ShaderFuncs::Dot(geometrySmith.GetArgument(0), geometrySmith.GetArgument(1)), ShaderLiteral::FromFloat(0.0f)));
		ShaderVariablePtr gNdotL = geometrySmith.DefineVar(ShaderFuncs::Max(ShaderFuncs::Dot(geometrySmith.GetArgument(0), geometrySmith.GetArgument(2)), ShaderLiteral::FromFloat(0.0f)));
		ShaderVariablePtr ggx2 = geometrySmith.DefineVar(ShaderFuncs::Call(geometrySchlickGGX, { gNdotV, geometrySmith.GetArgument(3) }));
		ShaderVariablePtr ggx1 = geometrySmith.DefineVar(ShaderFuncs::Call(geometrySchlickGGX, { gNdotL, geometrySmith.GetArgument(3) }));
		geometrySmith.Return(ShaderFuncs::Mul(ggx1, ggx2));

		IfScope& alphaThresholdTest = fragment.If(ShaderFuncs::LessThan(masterNodeValues.at("Alpha"), masterNodeValues.at("AlphaThreshold")));
		alphaThresholdTest.Discard();
		ShaderVariablePtr albedo = fragment.DefineVar(masterNodeValues.at("Albedo"));
		ShaderVariablePtr metallic = fragment.DefineVar(masterNodeValues.at("Metallic"));
		ShaderVariablePtr roughness = fragment.DefineVar(masterNodeValues.at("Roughness"));
		ShaderVariablePtr ao = fragment.DefineVar(masterNodeValues.at("Occlusion"));
		// Unit world space normal vector
		ShaderVariablePtr N = fragment.DefineVar(ShaderFuncs::Call(calcWorldNormal, { ShaderFuncs::Normalize(masterNodeValues.at("Normal")) }));
		// Unit to camera vector
		ShaderVariablePtr V = fragment.DefineVar(ShaderFuncs::Normalize(ShaderFuncs::Sub(cameraPosition, inWorldPos)));
		// Total radiance vector
		ShaderVariablePtr Lo = fragment.DefineVar(ShaderLiteral::FromVec3({ 0.0f, 0.0f, 0.0f }));
		ShaderVariablePtr totalAmbient = fragment.DefineVar(ShaderLiteral::FromVec3({ 0.0f, 0.0f, 0.0f }));

		ShaderVariablePtr counter = fragment.DeclareVar<int>();
		ForLoopScope& loop = fragment.For(counter, ShaderLiteral::FromInt(0), ShaderFuncs::LessThan(counter, lightCount), std::make_unique<AddAssignOp>(counter, ShaderLiteral::FromInt(1)));
		ShaderVariablePtr lightPosition = loop.DefineVar(ShaderFuncs::Index(lightPositions, counter));
		ShaderVariablePtr lightColor = loop.DefineVar(ShaderFuncs::xyz(ShaderFuncs::Index(lightColors, counter)));
		loop.MulAssign(lightColor, ShaderFuncs::Index(lightIntensities, counter));
		ShaderVariablePtr lightAmbient = loop.DefineVar(ShaderFuncs::Index(lightAmbients, counter));
		ShaderVariablePtr lightAttenuation = loop.DefineVar(ShaderFuncs::Index(lightAttenuations, counter));
		// Unit to light vector
		ShaderVariablePtr L = loop.DefineVar(ShaderFuncs::Normalize(ShaderFuncs::Sub(lightPosition, inWorldPos)));
		// Half vector
		ShaderVariablePtr H = loop.DefineVar(ShaderFuncs::Normalize(ShaderFuncs::Add(V, L)));
		ShaderVariablePtr distance = loop.DefineVar(ShaderFuncs::Length(ShaderFuncs::Sub(lightPosition, inWorldPos)));
		ShaderVariablePtr constAtt = loop.DefineVar(ShaderFuncs::x(lightAttenuation));
		ShaderVariablePtr linearAtt = loop.DefineVar(ShaderFuncs::Mul(ShaderFuncs::y(lightAttenuation), distance));
		ShaderVariablePtr quadAtt = loop.DefineVar(ShaderFuncs::Mul(ShaderFuncs::z(lightAttenuation), ShaderFuncs::Mul(distance, distance)));
		ShaderVariablePtr attenuation = loop.DefineVar(ShaderFuncs::Div(ShaderLiteral::FromFloat(1.0f), ShaderFuncs::Add(constAtt, ShaderFuncs::Add(linearAtt, quadAtt))));
		ShaderVariablePtr radiance = loop.DefineVar(ShaderFuncs::Mul(lightColor, attenuation));

		ShaderVariablePtr f0 = loop.DefineVar(ShaderLiteral::FromVec3({ 0.4f, 0.4f, 0.4f }));
		loop.SetVariable(f0, ShaderFuncs::Mix(f0, albedo, metallic));
		ShaderVariablePtr F = loop.DefineVar(ShaderFuncs::Call(fresnelSchlick, { ShaderFuncs::Max(ShaderFuncs::Dot(H, V), ShaderLiteral::FromFloat(0.0f)), f0 }));
		ShaderVariablePtr NDF = loop.DefineVar(ShaderFuncs::Call(distributionGGX, { N, H, roughness }));
		ShaderVariablePtr G = loop.DefineVar(ShaderFuncs::Call(geometrySmith, { N, V, L, roughness }));
		ShaderVariablePtr numerator = loop.DefineVar(ShaderFuncs::Mul(ShaderFuncs::Mul(NDF, G), F));
		ShaderVariablePtr maxNdotV = loop.DefineVar(ShaderFuncs::Max(ShaderFuncs::Dot(N, V), ShaderLiteral::FromFloat(0.0f)));
		ShaderVariablePtr maxNdotL = loop.DefineVar(ShaderFuncs::Max(ShaderFuncs::Dot(N, L), ShaderLiteral::FromFloat(0.0f)));
		ShaderVariablePtr denominator = loop.DefineVar(ShaderFuncs::Mul(ShaderLiteral::FromFloat(4.0f), ShaderFuncs::Mul(maxNdotV, maxNdotL)));
		ShaderVariablePtr specular = loop.DefineVar(ShaderFuncs::Div(numerator, ShaderFuncs::Max(denominator, ShaderLiteral::FromFloat(0.001f))));

		ShaderVariablePtr kS = loop.DefineVar(F);
		ShaderVariablePtr kD = loop.DefineVar(ShaderFuncs::Sub(ShaderLiteral::FromVec3({ 1.0f, 1.0f, 1.0f }), kS));
		loop.MulAssign(kD, ShaderFuncs::Sub(ShaderLiteral::FromFloat(1.0f), metallic));
		loop.AddAssign(Lo, ShaderFuncs::Mul(ShaderFuncs::Mul(ShaderFuncs::Add(ShaderFuncs::Div(ShaderFuncs::Mul(kD, albedo), ShaderLiteral::Pi()), specular), radiance), maxNdotL));
		loop.AddAssign(totalAmbient, ShaderFuncs::xyz(ShaderFuncs::Mul(ShaderFuncs::Index(lightAmbientColors, counter), lightAmbient)));

		ShaderVariablePtr color = fragment.DefineVar(ShaderFuncs::Add(ShaderFuncs::Mul(ShaderFuncs::Mul(totalAmbient, albedo), ao), Lo));
		fragment.SetVariable(color, ShaderFuncs::Div(color, ShaderFuncs::Add(color, ShaderLiteral::FromVec3({ 1.0f, 1.0f, 1.0f }))));
		fragment.SetVariable(color, ShaderFuncs::Pow(color, ShaderLiteral::FromVec3({ 1.0f / 2.2f, 1.0f / 2.2f, 1.0f / 2.2f })));

		fragment.SetFragColor(ShaderFuncs::Vec4(color, masterNodeValues.at("Alpha")));
	}

}