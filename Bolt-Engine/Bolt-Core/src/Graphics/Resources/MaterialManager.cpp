#include "bltpch.h"
#include "MaterialManager.h"
#include "Graphics/Resources/Meshes/Materials/Shaders/Components/ShaderFuncs.h"
#include "Graphics/Resources/Meshes/Materials/Shaders/Components/Operations/SetValueOp.h"
#include "Graphics/Resources/Meshes/Materials/Shaders/Components/Operations/MathOps/AddAssignOp.h"
#include "Graphics/Resources/Meshes/Materials/MaterialGraph/Nodes/Nodes.h"
#include "ResourceManager.h"

namespace Bolt
{

	MaterialManager::MaterialManager(ResourceManager* manager)
		: m_Manager(manager), m_DefaultGraph(manager), m_TextureGraph(manager), m_FontBuilder(true), m_DefaultLightingGraph(manager), m_TextureLightingGraph(manager), m_PBRGraph(manager), m_PBRTextureGraph(manager)
	{
		CreateDefaultMaterial(m_DefaultGraph);
		CreateTextureMaterial(m_TextureGraph);
		CreateFontMaterial(m_FontBuilder);
		CreateDefaultLightingMaterial(m_DefaultLightingGraph);
		CreateTextureLightingMaterial(m_TextureLightingGraph);
		CreatePBRMaterial(m_PBRGraph);
		CreatePBRTextureMaterial(m_PBRTextureGraph);
	}

	std::unique_ptr<Material> MaterialManager::Default(const Color& baseColor) const
	{
		std::unique_ptr<Material> material = m_DefaultGraph.GetMaterial();
		material->GetLinkContext().Link("Color", baseColor);
		material->SetIsTransparent(baseColor.a < 0.99f);
		return material;
	}

	std::unique_ptr<Material> MaterialManager::Texture(const ResourcePtr<Texture2D>& texture) const
	{
		std::unique_ptr<Material> material = m_TextureGraph.GetMaterial();
		material->GetLinkContext().Link("Texture", texture);
		return material;
	}

	std::unique_ptr<Material> MaterialManager::Font(const ResourcePtr<Bolt::Font>& font, const Color& baseColor) const
	{
		std::unique_ptr<Material> material = m_FontBuilder.BuildMaterial<Material>();
		material->GetLinkContext().Link("Font", font);
		material->GetLinkContext().Link("Color", baseColor);
		return material;
	}

	std::unique_ptr<Material> MaterialManager::DefaultLighting(const Color& color) const
	{
		std::unique_ptr<Material> material = m_DefaultLightingGraph.GetMaterial();
		material->GetLinkContext().Link("Color", color);
		return material;
	}

	std::unique_ptr<Material> MaterialManager::TextureLighting() const
	{
		std::unique_ptr<Material> material = m_TextureLightingGraph.GetMaterial();
		return material;
	}

	std::unique_ptr<PBRMaterial> MaterialManager::PBR() const
	{
		std::unique_ptr<PBRMaterial> material = m_PBRGraph.GetMaterial<PBRMaterial>();
		return material;
	}

	std::unique_ptr<PBRTextureMaterial> MaterialManager::PBRTexture() const
	{
		std::unique_ptr<PBRTextureMaterial> material = m_PBRTextureGraph.GetMaterial<PBRTextureMaterial>();
		return material;
	}

	void MaterialManager::CreateDefaultMaterial(BasicMaterialGraph& graph) const
	{
		PropertyNode& color = graph.AddProperty("Color", Color::White);
		SplitVec4Node& splitter = graph.AddNode<SplitVec4Node>();
		splitter.SetInput(color.GetValue());
		graph.SetRGB(splitter.GetRGB());
		graph.SetAlpha(splitter.GetA());
		graph.Build();
	}

	void MaterialManager::CreateTextureMaterial(BasicMaterialGraph& graph) const
	{
		PropertyNode& texture = graph.AddProperty("Texture", m_Manager->Textures().DefaultWhite());
		SampleTextureNode& sampler = graph.AddNode<SampleTextureNode>();
		sampler.SetTexture(texture.GetValue());
		graph.SetRGB(sampler.GetRGB());
		graph.SetAlpha(sampler.GetA());
		graph.Build();
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

	void MaterialManager::CreateDefaultLightingMaterial(LitMaterialGraph& graph) const
	{
		PropertyNode& color = graph.AddProperty("Color", Color::White);
		SplitVec4Node& splitter = graph.AddNode<SplitVec4Node>();
		splitter.SetInput(color.GetValue());
		graph.SetRGB(splitter.GetRGB());
		PropertyNode& shininess = graph.AddProperty("Shininess", 0.0f);
		graph.SetShininess(shininess.GetValue());
		PropertyNode& shineDamper = graph.AddProperty("ShineDamper", 10.0f);
		graph.SetShineDamper(shineDamper.GetValue());
		graph.SetAlpha(splitter.GetA());
		graph.Build();
	}

	void MaterialManager::CreateTextureLightingMaterial(LitMaterialGraph& graph) const
	{
		PropertyNode& color = graph.AddProperty("Texture", m_Manager->Textures().DefaultWhite());
		SampleTextureNode& sampler = graph.AddNode<SampleTextureNode>();
		sampler.SetTexture(color.GetValue());
		graph.SetRGB(sampler.GetRGB());
		PropertyNode& shininess = graph.AddProperty("Shininess", 0.0f);
		graph.SetShininess(shininess.GetValue());
		PropertyNode& shineDamper = graph.AddProperty("ShineDamper", 10.0f);
		graph.SetShineDamper(shineDamper.GetValue());
		graph.SetAlpha(sampler.GetA());
		graph.Build();
	}

	void MaterialManager::CreatePBRMaterial(PBRMaterialGraph& graph) const
	{
		PropertyNode& albedo = graph.AddProperty("Albedo", Color::White);
		SplitVec4Node& splitter = graph.AddNode<SplitVec4Node>();
		splitter.SetInput(albedo.GetValue());
		graph.SetRGB(splitter.GetRGB());
		PropertyNode& metallic = graph.AddProperty("Metallic", 0.5f);
		graph.SetMetallic(metallic.GetValue());
		PropertyNode& roughness = graph.AddProperty("Roughness", 0.5f);
		graph.SetRoughness(roughness.GetValue());
		PropertyNode& ao = graph.AddProperty("AO", 1.0f);
		graph.SetOcclusion(ao.GetValue());
		PropertyNode& normal = graph.AddProperty("Normal", Vector3f(0, 0, 1));
		graph.SetNormal(normal.GetValue());
		PropertyNode& alpha = graph.AddProperty("Alpha", 1.0f);
		graph.SetAlpha(alpha.GetValue());
		PropertyNode& alphaThreshold = graph.AddProperty("AlphaThreshold", 0.0f);
		graph.SetAlphaThreshold(alphaThreshold.GetValue());
		graph.Build();
	}

	void MaterialManager::CreatePBRTextureMaterial(PBRMaterialGraph& graph) const
	{
		PropertyNode& albedo = graph.AddProperty("Albedo", m_Manager->Textures().DefaultWhite());
		SampleTextureNode& albedoSampler = graph.AddNode(std::make_unique<SampleTextureNode>());
		albedoSampler.SetTexture(albedo.GetValue());
		graph.SetRGB(albedoSampler.GetRGB());
		PropertyNode& metallic = graph.AddProperty("Metallic", m_Manager->Textures().DefaultBlack());
		SampleTextureNode& metallicSampler = graph.AddNode(std::make_unique<SampleTextureNode>(SampleMode::Normal));
		metallicSampler.SetTexture(metallic.GetValue());
		graph.SetMetallic(metallicSampler.GetR());
		PropertyNode& roughness = graph.AddProperty("Roughness", m_Manager->Textures().DefaultWhite());
		SampleTextureNode& roughnessSampler = graph.AddNode(std::make_unique<SampleTextureNode>(SampleMode::Normal));
		roughnessSampler.SetTexture(roughness.GetValue());
		graph.SetRoughness(roughnessSampler.GetR());
		PropertyNode& ao = graph.AddProperty("AO", m_Manager->Textures().DefaultWhite());
		SampleTextureNode& aoSampler = graph.AddNode(std::make_unique<SampleTextureNode>(SampleMode::Normal));
		aoSampler.SetTexture(ao.GetValue());
		graph.SetOcclusion(aoSampler.GetR());
		PropertyNode& normal = graph.AddProperty("Normal", m_Manager->Textures().DefaultNormalMap());
		SampleTextureNode& normalSampler = graph.AddNode(std::make_unique<SampleTextureNode>(SampleMode::Normal));
		normalSampler.SetTexture(normal.GetValue());
		MultiplyNode& normalScaling = graph.AddNode<MultiplyNode>();
		normalScaling.SetInputA(normalSampler.GetRGB());
		normalScaling.SetInputB(graph.AddNode(std::make_unique<ConstantFloatNode>(2.0f)).GetValue());
		SubtractNode& normalSubtract = graph.AddNode<SubtractNode>();
		normalSubtract.SetInputA(normalScaling.GetResult());
		normalSubtract.SetInputB(graph.AddNode(std::make_unique<ConstantFloatNode>(1.0f)).GetValue());
		graph.SetNormal(normalSubtract.GetResult());
		PropertyNode& alpha = graph.AddProperty("Alpha", 1.0f);
		graph.SetAlpha(alpha.GetValue());
		PropertyNode& alphaThreshold = graph.AddProperty("AlphaThreshold", 0.0f);
		graph.SetAlphaThreshold(alphaThreshold.GetValue());
		graph.Build();
	}

}