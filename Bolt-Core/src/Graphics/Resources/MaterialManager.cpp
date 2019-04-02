#include "Types.h"
#include "MaterialManager.h"

namespace Bolt
{

	MaterialManager::MaterialManager()
		: m_DefaultBuilder(), m_TextureBuilder(), m_FontBuilder()
	{
		CreateDefaultMaterial(m_DefaultBuilder);
		CreateTextureMaterial(m_TextureBuilder);
		CreateFontMaterial(m_FontBuilder);
	}

	std::unique_ptr<Material> MaterialManager::Default(const Color& baseColor) const
	{
		std::unique_ptr<Material> material = m_DefaultBuilder.BuildMaterial();
		material->GetShader().Link("Color", baseColor);
		return material;
	}

	std::unique_ptr<Material> MaterialManager::Texture(const ResourcePtr<const Texture2D>& texture) const
	{
		std::unique_ptr<Material> material = m_TextureBuilder.BuildMaterial();
		material->GetShader().Link("Texture", texture);
		material->GetShader().Link("Color", Color::White);
		return material;
	}

	std::unique_ptr<Material> MaterialManager::Font(const ResourcePtr<const Bolt::Font>& font, const Color& baseColor) const
	{
		std::unique_ptr<Material> material = m_FontBuilder.BuildMaterial();
		material->GetShader().Link("Font", font);
		material->GetShader().Link("Color", baseColor);
		return material;
	}

	void MaterialManager::CreateDefaultMaterial(MaterialBuilder& builder) const
	{
		VertexShader& vertex = builder.Factory().Vertex();
		ShaderVariablePtr worldPosition = ShaderVariable::Create(ShaderFuncs::Mul(vertex.RendererUniform(RendererUniform::ModelMatrix), vertex.Position()));
		ShaderVariablePtr viewPosition = ShaderVariable::Create(ShaderFuncs::Mul(vertex.RendererUniform(RendererUniform::ViewMatrix), worldPosition));
		ShaderVariablePtr screenPosition = ShaderVariable::Create(ShaderFuncs::Mul(vertex.RendererUniform(RendererUniform::ProjectionMatrix), viewPosition));
		vertex.SetVertexPosition(screenPosition);
		ShaderPassValuePtr outColor = vertex.Pass(vertex.Color());
		FragmentShader& fragment = builder.Factory().Fragment();
		fragment.SetFragColor(ShaderFuncs::Mul(outColor, fragment.Uniform<Color>("Color")));
	}

	void MaterialManager::CreateTextureMaterial(MaterialBuilder& builder) const
	{
		VertexShader& vertex = builder.Factory().Vertex();
		ShaderVariablePtr worldPosition = ShaderVariable::Create(ShaderFuncs::Mul(vertex.RendererUniform(RendererUniform::ModelMatrix), vertex.Position()));
		ShaderVariablePtr viewPosition = ShaderVariable::Create(ShaderFuncs::Mul(vertex.RendererUniform(RendererUniform::ViewMatrix), worldPosition));
		ShaderVariablePtr screenPosition = ShaderVariable::Create(ShaderFuncs::Mul(vertex.RendererUniform(RendererUniform::ProjectionMatrix), viewPosition));
		vertex.SetVertexPosition(screenPosition);
		ShaderPassValuePtr outColor = vertex.Pass(vertex.Color());
		ShaderPassValuePtr outTexCoord = vertex.Pass(vertex.TexCoord());
		FragmentShader& fragment = builder.Factory().Fragment();
		ShaderUniformPtr texture = fragment.Uniform<Texture2D>("Texture");
		fragment.SetFragColor(ShaderFuncs::Mul(ShaderFuncs::Mul(outColor, fragment.Uniform<Color>("Color")), ShaderFuncs::SampleTexture(texture, outTexCoord)));
	}

	void MaterialManager::CreateFontMaterial(MaterialBuilder& builder) const
	{
		VertexShader& vertex = builder.Factory().Vertex();
		ShaderVariablePtr worldPosition = ShaderVariable::Create(ShaderFuncs::Mul(vertex.RendererUniform(RendererUniform::ModelMatrix), vertex.Position()));
		ShaderVariablePtr viewPosition = ShaderVariable::Create(ShaderFuncs::Mul(vertex.RendererUniform(RendererUniform::ViewMatrix), worldPosition));
		ShaderVariablePtr screenPosition = ShaderVariable::Create(ShaderFuncs::Mul(vertex.RendererUniform(RendererUniform::ProjectionMatrix), viewPosition));
		vertex.SetVertexPosition(screenPosition);
		ShaderPassValuePtr outColor = vertex.Pass(vertex.Color());
		ShaderPassValuePtr outTexCoord = vertex.Pass(vertex.TexCoord());
		FragmentShader& fragment = builder.Factory().Fragment();
		ShaderUniformPtr texture = fragment.Uniform<Texture2D>("Font");
		ShaderVariablePtr color = ShaderVariable::Create(ShaderFuncs::Mul(fragment.Uniform<Color>("Color"), outColor));
		fragment.SetFragColor(ShaderFuncs::Vec4(ShaderFuncs::x(color), ShaderFuncs::y(color), ShaderFuncs::z(color), ShaderFuncs::x(ShaderFuncs::SampleTexture(texture, outTexCoord))));
	}

}