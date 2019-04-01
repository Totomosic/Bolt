#include "Types.h"
#include "MaterialManager.h"

namespace Bolt
{

	MaterialManager::MaterialManager()
		: m_DefaultBuilder()
	{
		CreateDefaultMaterial(m_DefaultBuilder);
	}

	std::unique_ptr<Material> MaterialManager::Default(const Color& baseColor) const
	{
		std::unique_ptr<Material> material = m_DefaultBuilder.BuildMaterial();
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

}