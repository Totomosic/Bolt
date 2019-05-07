#include "Types.h"
#include "MaterialManager.h"
#include "Graphics/Resources/Meshes/Materials/Shaders/Components/ShaderFuncs.h"
#include "Graphics/Resources/Meshes/Materials/Shaders/Components/Operations/SetValueOp.h"
#include "Graphics/Resources/Meshes/Materials/Shaders/Components/Operations/MathOps/AddAssignOp.h"

namespace Bolt
{

	MaterialManager::MaterialManager()
		: m_DefaultBuilder(), m_TextureBuilder(), m_FontBuilder(), m_DefaultLightingBuilder()
	{
		CreateDefaultMaterial(m_DefaultBuilder);
		CreateTextureMaterial(m_TextureBuilder);
		CreateFontMaterial(m_FontBuilder);
		CreateDefaultLightingMaterial(m_DefaultLightingBuilder);
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

	std::unique_ptr<Material> MaterialManager::DefaultLighting(const Color& baseColor) const
	{
		std::unique_ptr<Material> material = m_DefaultLightingBuilder.BuildMaterial();
		material->GetShader().Link("Color", baseColor);
		return material;
	}

	void MaterialManager::CreateDefaultMaterial(MaterialBuilder& builder) const
	{
		VertexShader& vertex = builder.Factory().Vertex();
		ShaderVariablePtr modelMatrix = vertex.RendererUniform(RendererUniform::ModelMatrix);
		ShaderVariablePtr viewMatrix = vertex.RendererUniform(RendererUniform::ViewMatrix);
		ShaderVariablePtr projectionMatrix = vertex.RendererUniform(RendererUniform::ProjectionMatrix);
		ShaderVariablePtr outColor = vertex.DeclarePassOut<Color>();
		ShaderVariablePtr worldPos = vertex.DefineVar(ShaderFuncs::Mul(modelMatrix, vertex.Position()));
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
		ShaderVariablePtr worldPos = vertex.DefineVar(ShaderFuncs::Mul(modelMatrix, vertex.Position()));
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
		ShaderVariablePtr worldPos = vertex.DefineVar(ShaderFuncs::Mul(modelMatrix, vertex.Position()));
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
		ShaderVariablePtr worldPos = vertex.DefineVar(ShaderFuncs::Mul(modelMatrix, vertex.Position()));
		ShaderVariablePtr viewPos = vertex.DefineVar(ShaderFuncs::Mul(viewMatrix, worldPos));
		ShaderVariablePtr screenPos = vertex.DefineVar(ShaderFuncs::Mul(projectionMatrix, viewPos));
		vertex.SetVertexPosition(screenPos);
		vertex.SetVariable(outColor, vertex.Color());
		ShaderVariablePtr invViewMatrix = vertex.DefineVar(ShaderFuncs::Inverse(viewMatrix));
		vertex.SetVariable(outToCamera, ShaderFuncs::Sub(ShaderFuncs::xyz(ShaderFuncs::Index(invViewMatrix, ShaderLiteral::FromInt(3))), ShaderFuncs::xyz(worldPos)));
		vertex.SetVariable(outWorldNormal, ShaderFuncs::xyz(ShaderFuncs::Mul(modelMatrix, ShaderFuncs::Vec4(vertex.Normal(), ShaderLiteral::FromFloat(0)))));

		FragmentShader& fragment = builder.Factory().Fragment();
		ShaderVariablePtr inColor = fragment.DeclarePassIn(outColor);
		ShaderVariablePtr inToCamera = fragment.DeclarePassIn(outToCamera);
		ShaderVariablePtr inWorldNormal = fragment.DeclarePassIn(outWorldNormal);
		ShaderVariablePtr lightPositions = fragment.RendererUniform(RendererUniform::LightPositions);
		ShaderVariablePtr lightColors = fragment.RendererUniform(RendererUniform::LightColors);
		ShaderVariablePtr lightTypes = fragment.RendererUniform(RendererUniform::LightTypes);
		ShaderVariablePtr lightDirections = fragment.RendererUniform(RendererUniform::LightDirections);
		ShaderVariablePtr lightAmbients = fragment.RendererUniform(RendererUniform::LightAmbients);
		ShaderVariablePtr lightCount = fragment.RendererUniform(RendererUniform::LightCount);
		ShaderVariablePtr color = fragment.Uniform<Color>("Color");
		fragment.SetFragColor(ShaderFuncs::Mul(outColor, color));
		ShaderVariablePtr test = fragment.DefineVar(lightCount);
		fragment.DivAssign(test, test);
		fragment.MulAssign(fragment.FragColor(), ShaderFuncs::Mul(ShaderLiteral::FromVec4({ 1, 1, 1, 1 }), test));
	}

}