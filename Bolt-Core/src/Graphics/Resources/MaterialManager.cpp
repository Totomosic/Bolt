#include "bltpch.h"
#include "MaterialManager.h"
#include "Graphics/Resources/Meshes/Materials/Shaders/Components/ShaderFuncs.h"
#include "Graphics/Resources/Meshes/Materials/Shaders/Components/Operations/SetValueOp.h"
#include "Graphics/Resources/Meshes/Materials/Shaders/Components/Operations/MathOps/AddAssignOp.h"

namespace Bolt
{

	MaterialManager::MaterialManager()
		: m_DefaultBuilder(false), m_TextureBuilder(false), m_FontBuilder(true), m_DefaultLightingBuilder(false), m_PBRBuilder(false), m_PBRTextureBuilder(false)
	{
		CreateDefaultMaterial(m_DefaultBuilder);
		CreateTextureMaterial(m_TextureBuilder);
		CreateFontMaterial(m_FontBuilder);
		CreateDefaultLightingMaterial(m_DefaultLightingBuilder);
		CreatePBRMaterial(m_PBRBuilder);
		CreatePBRTextureMaterial(m_PBRTextureBuilder);
	}

	std::unique_ptr<Material> MaterialManager::Default(const Color& baseColor) const
	{
		std::unique_ptr<Material> material = m_DefaultBuilder.BuildMaterial();
		material->GetShader().Link("Color", baseColor);
		return material;
	}

	std::unique_ptr<Material> MaterialManager::Texture(const ResourcePtr<Texture2D>& texture) const
	{
		std::unique_ptr<Material> material = m_TextureBuilder.BuildMaterial();
		material->GetShader().Link("Texture", texture);
		material->GetShader().Link("Color", Color::White);
		return material;
	}

	std::unique_ptr<Material> MaterialManager::Font(const ResourcePtr<Bolt::Font>& font, const Color& baseColor) const
	{
		std::unique_ptr<Material> material = m_FontBuilder.BuildMaterial();
		material->GetShader().Link("Font", font);
		material->GetShader().Link("Color", baseColor);
		return material;
	}

	std::unique_ptr<Material> MaterialManager::DefaultLighting(const Color& baseColor) const
	{
		std::unique_ptr<Material> material = m_DefaultLightingBuilder.BuildMaterial();
		material->GetShader().Link("Color", baseColor);
		return material;
	}

	std::unique_ptr<Material> MaterialManager::PBR() const
	{
		std::unique_ptr<Material> material = m_PBRBuilder.BuildMaterial();
		return material;
	}

	std::unique_ptr<Material> MaterialManager::PBRTexture() const
	{
		std::unique_ptr<Material> material = m_PBRTextureBuilder.BuildMaterial();
		return material;
	}

	void MaterialManager::CreateDefaultMaterial(MaterialBuilder& builder) const
	{
		VertexShader& vertex = builder.Factory().Vertex();
		ShaderVariablePtr modelMatrix = vertex.RendererUniform(RendererUniform::ModelMatrix);
		ShaderVariablePtr viewMatrix = vertex.RendererUniform(RendererUniform::ViewMatrix);
		ShaderVariablePtr projectionMatrix = vertex.RendererUniform(RendererUniform::ProjectionMatrix);
		ShaderVariablePtr outColor = vertex.DeclarePassOut<Color>();

		ShaderVariablePtr position = vertex.DefineVar(ShaderFuncs::Vec4(vertex.Position(), ShaderLiteral::FromFloat(1.0f)));
		ShaderVariablePtr worldPos = vertex.DefineVar(ShaderFuncs::Mul(modelMatrix, position));
		ShaderVariablePtr viewPos = vertex.DefineVar(ShaderFuncs::Mul(viewMatrix, worldPos));
		ShaderVariablePtr screenPos = vertex.DefineVar(ShaderFuncs::Mul(projectionMatrix, viewPos));
		vertex.SetVertexPosition(screenPos);
		vertex.SetVariable(outColor, vertex.Color());

		FragmentShader& fragment = builder.Factory().Fragment();
		ShaderVariablePtr inColor = fragment.DeclarePassIn(outColor);
		ShaderVariablePtr color = fragment.Uniform<Color>("Color");
		fragment.SetFragColor(ShaderFuncs::Mul(outColor, color));
	}

	void MaterialManager::CreateTextureMaterial(MaterialBuilder& builder) const
	{
		VertexShader& vertex = builder.Factory().Vertex();
		ShaderVariablePtr modelMatrix = vertex.RendererUniform(RendererUniform::ModelMatrix);
		ShaderVariablePtr viewMatrix = vertex.RendererUniform(RendererUniform::ViewMatrix);
		ShaderVariablePtr projectionMatrix = vertex.RendererUniform(RendererUniform::ProjectionMatrix);
		ShaderVariablePtr texCoordMatrix = vertex.Uniform<Matrix3f>("TexCoordMatrix", Matrix3f::Identity());
		ShaderVariablePtr outColor = vertex.DeclarePassOut<Color>();
		ShaderVariablePtr outTexCoord = vertex.DeclarePassOut<Vector2f>();

		ShaderVariablePtr position = vertex.DefineVar(ShaderFuncs::Vec4(vertex.Position(), ShaderLiteral::FromFloat(1.0f)));
		ShaderVariablePtr worldPos = vertex.DefineVar(ShaderFuncs::Mul(modelMatrix, position));
		ShaderVariablePtr viewPos = vertex.DefineVar(ShaderFuncs::Mul(viewMatrix, worldPos));
		ShaderVariablePtr screenPos = vertex.DefineVar(ShaderFuncs::Mul(projectionMatrix, viewPos));
		vertex.SetVertexPosition(screenPos);
		vertex.SetVariable(outColor, vertex.Color());
		vertex.SetVariable(outTexCoord, ShaderFuncs::xy(ShaderFuncs::Mul(texCoordMatrix, ShaderFuncs::Vec3(vertex.TexCoord(), ShaderLiteral::FromFloat(0.0f)))));

		FragmentShader& fragment = builder.Factory().Fragment();
		ShaderVariablePtr inColor = fragment.DeclarePassIn(outColor);
		ShaderVariablePtr inTexCoord = fragment.DeclarePassIn(outTexCoord);
		ShaderVariablePtr color = fragment.Uniform<Color>("Color");
		ShaderVariablePtr texture = fragment.Uniform<Texture2D>("Texture");
		fragment.SetFragColor(ShaderFuncs::Mul(outColor, ShaderFuncs::Mul(ShaderFuncs::SampleTexture(texture, inTexCoord), color)));
	}

	void MaterialManager::CreateFontMaterial(MaterialBuilder& builder) const
	{
		VertexShader& vertex = builder.Factory().Vertex();
		ShaderVariablePtr modelMatrix = vertex.RendererUniform(RendererUniform::ModelMatrix);
		ShaderVariablePtr viewMatrix = vertex.RendererUniform(RendererUniform::ViewMatrix);
		ShaderVariablePtr projectionMatrix = vertex.RendererUniform(RendererUniform::ProjectionMatrix);
		ShaderVariablePtr outColor = vertex.DeclarePassOut<Color>();
		ShaderVariablePtr outTexCoord = vertex.DeclarePassOut<Vector2f>();

		ShaderVariablePtr position = vertex.DefineVar(ShaderFuncs::Vec4(vertex.Position(), ShaderLiteral::FromFloat(1.0f)));
		ShaderVariablePtr worldPos = vertex.DefineVar(ShaderFuncs::Mul(modelMatrix, position));
		ShaderVariablePtr viewPos = vertex.DefineVar(ShaderFuncs::Mul(viewMatrix, worldPos));
		ShaderVariablePtr screenPos = vertex.DefineVar(ShaderFuncs::Mul(projectionMatrix, viewPos));
		vertex.SetVertexPosition(screenPos);
		vertex.SetVariable(outColor, vertex.Color());
		vertex.SetVariable(outTexCoord, vertex.TexCoord());

		FragmentShader& fragment = builder.Factory().Fragment();
		ShaderVariablePtr inColor = fragment.DeclarePassIn(outColor);
		ShaderVariablePtr inTexCoord = fragment.DeclarePassIn(outTexCoord);
		ShaderVariablePtr color = fragment.Uniform<Color>("Color");
		ShaderVariablePtr font = fragment.Uniform<Texture2D>("Font");
		fragment.SetFragColor(ShaderFuncs::Vec4(ShaderFuncs::x(color), ShaderFuncs::y(color), ShaderFuncs::z(color), ShaderFuncs::x(ShaderFuncs::SampleTexture(font, outTexCoord))));
	}

	void MaterialManager::CreateDefaultLightingMaterial(MaterialBuilder& builder) const
	{
		VertexShader& vertex = builder.Factory().Vertex();
		ShaderVariablePtr modelMatrix = vertex.RendererUniform(RendererUniform::ModelMatrix);
		ShaderVariablePtr viewMatrix = vertex.RendererUniform(RendererUniform::ViewMatrix);
		ShaderVariablePtr projectionMatrix = vertex.RendererUniform(RendererUniform::ProjectionMatrix);
		ShaderVariablePtr outColor = vertex.DeclarePassOut<Color>();
		ShaderVariablePtr outToCamera = vertex.DeclarePassOut<Vector3f>();
		ShaderVariablePtr outWorldNormal = vertex.DeclarePassOut<Vector3f>();
		ShaderVariablePtr outWorldPos = vertex.DeclarePassOut<Vector3f>();

		ShaderVariablePtr position = vertex.DefineVar(ShaderFuncs::Vec4(vertex.Position(), ShaderLiteral::FromFloat(1.0f)));
		ShaderVariablePtr worldPos = vertex.DefineVar(ShaderFuncs::Mul(modelMatrix, position));
		ShaderVariablePtr viewPos = vertex.DefineVar(ShaderFuncs::Mul(viewMatrix, worldPos));
		ShaderVariablePtr screenPos = vertex.DefineVar(ShaderFuncs::Mul(projectionMatrix, viewPos));
		vertex.SetVertexPosition(screenPos);
		vertex.SetVariable(outColor, vertex.Color());
		ShaderVariablePtr invViewMatrix = vertex.DefineVar(ShaderFuncs::Inverse(viewMatrix));
		vertex.SetVariable(outToCamera, ShaderFuncs::Sub(ShaderFuncs::xyz(ShaderFuncs::Index(invViewMatrix, ShaderLiteral::FromInt(3))), ShaderFuncs::xyz(worldPos)));
		vertex.SetVariable(outWorldNormal, ShaderFuncs::xyz(ShaderFuncs::Mul(modelMatrix, ShaderFuncs::Vec4(vertex.Normal(), ShaderLiteral::FromFloat(0)))));
		vertex.SetVariable(outWorldPos, ShaderFuncs::xyz(worldPos));

		FragmentShader& fragment = builder.Factory().Fragment();
		ShaderVariablePtr inColor = fragment.DeclarePassIn(outColor);
		ShaderVariablePtr inToCamera = fragment.DeclarePassIn(outToCamera);
		ShaderVariablePtr inWorldNormal = fragment.DeclarePassIn(outWorldNormal);
		ShaderVariablePtr inWorldPos = fragment.DeclarePassIn(outWorldPos);
		ShaderVariablePtr lightPositions = fragment.RendererUniform(RendererUniform::LightPositions);
		ShaderVariablePtr lightColors = fragment.RendererUniform(RendererUniform::LightColors);
		ShaderVariablePtr lightAmbients = fragment.RendererUniform(RendererUniform::LightAmbients);
		ShaderVariablePtr lightCount = fragment.RendererUniform(RendererUniform::LightCount);
		ShaderVariablePtr color = fragment.Uniform<Color>("Color");

		ShaderVariablePtr reflectivity = fragment.Uniform<float>("Reflectivity", 0.0f);
		ShaderVariablePtr shineDamper = fragment.Uniform<float>("ShineDamper", 10.0f);
		ShaderVariablePtr specularHighlight = fragment.Uniform<Color>("SpecularHighlight", Color::White);

		ShaderVariablePtr totalDiffuse = fragment.DefineVar(ShaderLiteral::FromVec4({ 0.0f, 0.0f, 0.0f, 0.0f }));
		ShaderVariablePtr totalSpecular = fragment.DefineVar(ShaderLiteral::FromVec4({ 0.0f, 0.0f, 0.0f, 0.0f }));
		ShaderVariablePtr counter = fragment.DeclareVar<int>();
		ForLoopScope& loop = fragment.For(counter, ShaderLiteral::FromInt(0), ShaderFuncs::LessThan(counter, lightCount), std::make_unique<AddAssignOp>(counter, ShaderLiteral::FromInt(1)));
		ShaderVariablePtr unitToLightVec = loop.DefineVar(ShaderFuncs::Normalize(ShaderFuncs::Sub(ShaderFuncs::Index(lightPositions, counter), inWorldPos)));
		ShaderVariablePtr unitToCamVec = loop.DefineVar(ShaderFuncs::Normalize(inToCamera));
		ShaderVariablePtr unitNormal = loop.DefineVar(ShaderFuncs::Normalize(inWorldNormal));
		ShaderVariablePtr reflectedLight = loop.DefineVar(ShaderFuncs::Reflect(ShaderFuncs::Neg(unitToLightVec), unitNormal));
		ShaderVariablePtr nDotl = loop.DefineVar(ShaderFuncs::Dot(unitNormal, unitToLightVec));
		ShaderVariablePtr multiplier = loop.DefineVar(ShaderFuncs::Max(nDotl, ShaderFuncs::Index(lightAmbients, counter)));
		ShaderVariablePtr specularFactor = loop.DefineVar(ShaderFuncs::Max(ShaderFuncs::Dot(reflectedLight, unitToCamVec), ShaderLiteral::FromFloat(0.0f)));
		ShaderVariablePtr dampedSpecFactor = loop.DefineVar(ShaderFuncs::Pow(specularFactor, shineDamper));
		loop.AddAssign(totalDiffuse, ShaderFuncs::Vec4(ShaderFuncs::xyz(ShaderFuncs::Mul(ShaderFuncs::Index(lightColors, counter), multiplier)), ShaderLiteral::FromFloat(1)));
		loop.AddAssign(totalSpecular, ShaderFuncs::Vec4(ShaderFuncs::xyz(ShaderFuncs::Mul(ShaderFuncs::Mul(ShaderFuncs::Mul(ShaderFuncs::Index(lightColors, counter), dampedSpecFactor), reflectivity), specularHighlight)), ShaderLiteral::FromFloat(0)));
		fragment.SetFragColor(ShaderFuncs::Mul(ShaderFuncs::Add(totalDiffuse, totalSpecular), color));
	}

	void MaterialManager::CreatePBRMaterial(MaterialBuilder& builder) const
	{
		VertexShader& vertex = builder.Factory().Vertex();
		ShaderVariablePtr modelMatrix = vertex.RendererUniform(RendererUniform::ModelMatrix);
		ShaderVariablePtr viewMatrix = vertex.RendererUniform(RendererUniform::ViewMatrix);
		ShaderVariablePtr projectionMatrix = vertex.RendererUniform(RendererUniform::ProjectionMatrix);
		ShaderVariablePtr outTexCoord = vertex.DeclarePassOut(ValueType::Vector2f);
		ShaderVariablePtr outWorldPos = vertex.DeclarePassOut(ValueType::Vector3f);
		ShaderVariablePtr outWorldNormal = vertex.DeclarePassOut(ValueType::Vector3f);

		ShaderVariablePtr position = vertex.DefineVar(ShaderFuncs::Vec4(vertex.Position(), ShaderLiteral::FromFloat(1.0f)));
		ShaderVariablePtr worldPosition = vertex.DefineVar(ShaderFuncs::Mul(modelMatrix, position));
		ShaderVariablePtr viewPosition = vertex.DefineVar(ShaderFuncs::Mul(viewMatrix, worldPosition));
		ShaderVariablePtr screenPosition = vertex.DefineVar(ShaderFuncs::Mul(projectionMatrix, viewPosition));
		vertex.SetVertexPosition(screenPosition);
		vertex.SetVariable(outTexCoord, vertex.TexCoord());
		vertex.SetVariable(outWorldPos, ShaderFuncs::xyz(worldPosition));
		vertex.SetVariable(outWorldNormal, ShaderFuncs::xyz(ShaderFuncs::Mul(modelMatrix, ShaderFuncs::Vec4(vertex.Normal(), ShaderLiteral::FromFloat(0.0f)))));

		FragmentShader& fragment = builder.Factory().Fragment();
		ShaderVariablePtr inTexCoord = fragment.DeclarePassIn(outTexCoord);
		ShaderVariablePtr inWorldPos = fragment.DeclarePassIn(outWorldPos);
		ShaderVariablePtr inWorldNormal = fragment.DeclarePassIn(outWorldNormal);
		ShaderVariablePtr cameraPosition = fragment.RendererUniform(RendererUniform::CameraPosition);
		ShaderVariablePtr lightPositions = fragment.RendererUniform(RendererUniform::LightPositions);
		ShaderVariablePtr lightColors = fragment.RendererUniform(RendererUniform::LightColors);
		ShaderVariablePtr lightAmbients = fragment.RendererUniform(RendererUniform::LightAmbients);
		ShaderVariablePtr lightIntensities = fragment.RendererUniform(RendererUniform::LightIntensities);
		ShaderVariablePtr lightAttenuations = fragment.RendererUniform(RendererUniform::LightAttenuations);
		ShaderVariablePtr lightCount = fragment.RendererUniform(RendererUniform::LightCount);

		ShaderVariablePtr albedoColor = fragment.Uniform<Color>("Albedo", Color::White);
		ShaderVariablePtr metallic = fragment.Uniform<float>("Metallic", 0.0f);
		ShaderVariablePtr roughness = fragment.Uniform<float>("Roughness", 0.5f);
		ShaderVariablePtr ao = fragment.Uniform<float>("AO", 1.0f);

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

		ShaderVariablePtr albedo = fragment.DefineVar(ShaderFuncs::xyz(albedoColor));
		// Unit normal vector
		ShaderVariablePtr N = fragment.DefineVar(ShaderFuncs::Normalize(inWorldNormal));
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
		loop.AddAssign(totalAmbient, lightAmbient);

		ShaderVariablePtr color = fragment.DefineVar(ShaderFuncs::Add(ShaderFuncs::Mul(ShaderFuncs::Mul(totalAmbient, albedo), ao), Lo));
		fragment.SetVariable(color, ShaderFuncs::Div(color, ShaderFuncs::Add(color, ShaderLiteral::FromVec3({ 1.0f, 1.0f, 1.0f }))));
		fragment.SetVariable(color, ShaderFuncs::Pow(color, ShaderLiteral::FromVec3({ 1.0f / 2.2f, 1.0f / 2.2f, 1.0f / 2.2f })));

		fragment.SetFragColor(ShaderFuncs::Vec4(color, ShaderLiteral::FromFloat(1.0f)));
	}

	void MaterialManager::CreatePBRTextureMaterial(MaterialBuilder& builder) const
	{
		VertexShader& vertex = builder.Factory().Vertex();
		ShaderVariablePtr modelMatrix = vertex.RendererUniform(RendererUniform::ModelMatrix);
		ShaderVariablePtr viewMatrix = vertex.RendererUniform(RendererUniform::ViewMatrix);
		ShaderVariablePtr projectionMatrix = vertex.RendererUniform(RendererUniform::ProjectionMatrix);
		ShaderVariablePtr outTexCoord = vertex.DeclarePassOut(ValueType::Vector2f);
		ShaderVariablePtr outWorldPos = vertex.DeclarePassOut(ValueType::Vector3f);
		ShaderVariablePtr outWorldNormal = vertex.DeclarePassOut(ValueType::Vector3f);

		ShaderVariablePtr position = vertex.DefineVar(ShaderFuncs::Vec4(vertex.Position(), ShaderLiteral::FromFloat(1.0f)));
		ShaderVariablePtr worldPosition = vertex.DefineVar(ShaderFuncs::Mul(modelMatrix, position));
		ShaderVariablePtr viewPosition = vertex.DefineVar(ShaderFuncs::Mul(viewMatrix, worldPosition));
		ShaderVariablePtr screenPosition = vertex.DefineVar(ShaderFuncs::Mul(projectionMatrix, viewPosition));
		vertex.SetVertexPosition(screenPosition);
		vertex.SetVariable(outTexCoord, vertex.TexCoord());
		vertex.SetVariable(outWorldPos, ShaderFuncs::xyz(worldPosition));
		vertex.SetVariable(outWorldNormal, ShaderFuncs::xyz(ShaderFuncs::Mul(modelMatrix, ShaderFuncs::Vec4(vertex.Normal(), ShaderLiteral::FromFloat(0.0f)))));

		FragmentShader& fragment = builder.Factory().Fragment();
		ShaderVariablePtr inTexCoord = fragment.DeclarePassIn(outTexCoord);
		ShaderVariablePtr inWorldPos = fragment.DeclarePassIn(outWorldPos);
		ShaderVariablePtr inWorldNormal = fragment.DeclarePassIn(outWorldNormal);
		ShaderVariablePtr cameraPosition = fragment.RendererUniform(RendererUniform::CameraPosition);
		ShaderVariablePtr lightPositions = fragment.RendererUniform(RendererUniform::LightPositions);
		ShaderVariablePtr lightColors = fragment.RendererUniform(RendererUniform::LightColors);
		ShaderVariablePtr lightAmbients = fragment.RendererUniform(RendererUniform::LightAmbients);
		ShaderVariablePtr lightIntensities = fragment.RendererUniform(RendererUniform::LightIntensities);
		ShaderVariablePtr lightAttenuations = fragment.RendererUniform(RendererUniform::LightAttenuations);
		ShaderVariablePtr lightCount = fragment.RendererUniform(RendererUniform::LightCount);

		ShaderVariablePtr albedoMap = fragment.Uniform<Texture2D>("Albedo");
		ShaderVariablePtr metallicMap = fragment.Uniform<Texture2D>("Metallic");
		ShaderVariablePtr roughnessMap = fragment.Uniform<Texture2D>("Roughness");
		ShaderVariablePtr aoMap = fragment.Uniform<Texture2D>("AO");

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

		ShaderVariablePtr albedo = fragment.DefineVar(ShaderFuncs::Pow(ShaderFuncs::xyz(ShaderFuncs::SampleTexture(albedoMap, inTexCoord)), ShaderLiteral::FromVec3({ 2.2f, 2.2f, 2.2f })));
		ShaderVariablePtr metallic = fragment.DefineVar(ShaderFuncs::x(ShaderFuncs::SampleTexture(metallicMap, inTexCoord)));
		ShaderVariablePtr roughness = fragment.DefineVar(ShaderFuncs::x(ShaderFuncs::SampleTexture(roughnessMap, inTexCoord)));
		ShaderVariablePtr ao = fragment.DefineVar(ShaderLiteral::FromFloat(1.0f));//fragment.DefineVar(ShaderFuncs::x(ShaderFuncs::SampleTexture(aoMap, inTexCoord)));
		// Unit normal vector
		ShaderVariablePtr N = fragment.DefineVar(ShaderFuncs::Normalize(inWorldNormal));
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
		loop.AddAssign(totalAmbient, lightAmbient);

		ShaderVariablePtr color = fragment.DefineVar(ShaderFuncs::Add(ShaderFuncs::Mul(ShaderFuncs::Mul(totalAmbient, albedo), ao), Lo));
		fragment.SetVariable(color, ShaderFuncs::Div(color, ShaderFuncs::Add(color, ShaderLiteral::FromVec3({ 1.0f, 1.0f, 1.0f }))));
		fragment.SetVariable(color, ShaderFuncs::Pow(color, ShaderLiteral::FromVec3({ 1.0f / 2.2f, 1.0f / 2.2f, 1.0f / 2.2f })));

		fragment.SetFragColor(ShaderFuncs::Vec4(color, ShaderLiteral::FromFloat(1.0f)));
	}

}