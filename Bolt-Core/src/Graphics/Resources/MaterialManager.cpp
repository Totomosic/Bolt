#include "bltpch.h"
#include "MaterialManager.h"
#include "Graphics/Resources/Meshes/Materials/Shaders/Components/ShaderFuncs.h"
#include "Graphics/Resources/Meshes/Materials/Shaders/Components/Operations/SetValueOp.h"
#include "Graphics/Resources/Meshes/Materials/Shaders/Components/Operations/MathOps/AddAssignOp.h"
#include "Graphics/Resources/Meshes/Materials/MaterialGraph/Nodes/Nodes.h"
#include "ResourceManager.h"

namespace Bolt
{

	MaterialManager::MaterialManager()
		: m_DefaultGraph(), m_TextureGraph(), m_FontBuilder(true), m_DefaultLightingGraph(), m_TextureLightingGraph(), m_PBRGraph(), m_PBRTextureGraph()
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
		std::unique_ptr<Material> material = m_FontBuilder.BuildMaterial();
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

	std::unique_ptr<Material> MaterialManager::PBR() const
	{
		std::unique_ptr<Material> material = m_PBRGraph.GetMaterial();
		return material;
	}

	std::unique_ptr<Material> MaterialManager::PBRTexture() const
	{
		std::unique_ptr<Material> material = m_PBRTextureGraph.GetMaterial();
		return material;
	}

	void MaterialManager::CreateDefaultMaterial(BasicMaterialGraph& graph) const
	{
		PropertyNode& color = graph.AddProperty("Color", Color::White);
		graph.SetColor(color.GetValue());
		graph.Build();
	}

	void MaterialManager::CreateTextureMaterial(BasicMaterialGraph& graph) const
	{
		PropertyNode& texture = graph.AddProperty("Texture", ResourceManager::Get().Textures().DefaultWhite());
		SampleTextureNode& sampler = graph.AddNode<SampleTextureNode>();
		sampler.SetTexture(texture.GetValue());
		graph.SetColor(sampler.GetRGBA());
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
		graph.SetColor(color.GetValue());
		PropertyNode& shininess = graph.AddProperty("Shininess", 0.0f);
		graph.SetShininess(shininess.GetValue());
		PropertyNode& shineDamper = graph.AddProperty("ShineDamper", 10.0f);
		graph.SetShineDamper(shineDamper.GetValue());
		graph.Build();
	}

	void MaterialManager::CreateTextureLightingMaterial(LitMaterialGraph& graph) const
	{
		PropertyNode& color = graph.AddProperty("Color", ResourceManager::Get().Textures().DefaultWhite());
		SampleTextureNode& sampler = graph.AddNode<SampleTextureNode>();
		sampler.SetTexture(color.GetValue());
		graph.SetColor(sampler.GetRGBA());
		PropertyNode& shininess = graph.AddProperty("Shininess", 0.0f);
		graph.SetShininess(shininess.GetValue());
		PropertyNode& shineDamper = graph.AddProperty("ShineDamper", 10.0f);
		graph.SetShineDamper(shineDamper.GetValue());
		graph.Build();
	}

	void MaterialManager::CreatePBRMaterial(PBRMaterialGraph& graph) const
	{
		PropertyNode& albedo = graph.AddProperty("Albedo", Color::White);
		graph.SetAlbedo(albedo.GetValue());
		PropertyNode& metallic = graph.AddProperty("Metallic", 0.0f);
		graph.SetMetallic(metallic.GetValue());
		PropertyNode& roughness = graph.AddProperty("Roughness", 0.5f);
		graph.SetRoughness(roughness.GetValue());
		PropertyNode& ao = graph.AddProperty("AO", 1.0f);
		graph.SetOcclusion(ao.GetValue());
		graph.Build();
	}

	void MaterialManager::CreatePBRTextureMaterial(PBRMaterialGraph& graph) const
	{
		PropertyNode& albedo = graph.AddProperty("Albedo", ResourceManager::Get().Textures().DefaultWhite());
		SampleTextureNode& albedoSampler = graph.AddNode(std::make_unique<SampleTextureNode>());
		albedoSampler.SetTexture(albedo.GetValue());
		graph.SetAlbedo(albedoSampler.GetRGBA());
		PropertyNode& metallic = graph.AddProperty("Metallic", ResourceManager::Get().Textures().DefaultBlack());
		SampleTextureNode& metallicSampler = graph.AddNode(std::make_unique<SampleTextureNode>(SampleMode::Normal));
		metallicSampler.SetTexture(metallic.GetValue());
		graph.SetMetallic(metallicSampler.GetR());
		PropertyNode& roughness = graph.AddProperty("Roughness", ResourceManager::Get().Textures().DefaultWhite());
		SampleTextureNode& roughnessSampler = graph.AddNode(std::make_unique<SampleTextureNode>(SampleMode::Normal));
		roughnessSampler.SetTexture(roughness.GetValue());
		graph.SetRoughness(roughnessSampler.GetR());
		PropertyNode& ao = graph.AddProperty("AO", ResourceManager::Get().Textures().DefaultWhite());
		SampleTextureNode& aoSampler = graph.AddNode(std::make_unique<SampleTextureNode>(SampleMode::Normal));
		aoSampler.SetTexture(ao.GetValue());
		graph.SetOcclusion(aoSampler.GetR());
		graph.Build();
	}

}