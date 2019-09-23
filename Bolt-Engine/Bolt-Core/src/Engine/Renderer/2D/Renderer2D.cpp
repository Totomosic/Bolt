#include "bltpch.h"
#include "Renderer2D.h"
#include "Graphics/Resources/Meshes/Materials/Shaders/ShaderFactory.h"
#include "Graphics/Resources/Meshes/Materials/Shaders/Components/ShaderFuncs.h"

namespace Bolt
{

	Renderer2D::Renderer2D()
		: m_Layout(), m_TexturesPerDraw(32), m_SpritesPerDraw(10000), m_Batches(), m_TextureBatches(), m_CurrentBatch(nullptr), m_DefaultShader(), m_TextureShader()
	{
		m_Layout.AddAttribute<Vector3f>(POSITION_ATTRIBUTE_INDEX, 1);
		m_Layout.AddAttribute<Vector2f>(TEXCOORD_ATTRIBUTE_INDEX, 1);
		m_Layout.AddAttribute<byte>(COLOR_ATTRIBUTE_INDEX, 4, true);
		m_Layout.AddAttribute<int>(TEXTUREID_ATTRIBUTE_INDEX, 1);

		CreateDefaultShader();
		CreateTextureShader();
	}

	int Renderer2D::GetSpritesPerDraw() const
	{
		return m_SpritesPerDraw;
	}

	void Renderer2D::SetSpritesPerDraw(int spriteCount)
	{

	}

	void Renderer2D::BeginScene(const RenderCamera& camera, const Framebuffer* renderTarget)
	{

	}

	void Renderer2D::EndScene()
	{

	}

	void Renderer2D::Flush()
	{

	}

	void Renderer2D::DrawSprite(float x0, float y0, float width, float height, const Color& color, const ResourcePtr<Texture2D>& texture, const TextureFrame& frame)
	{

	}

	Renderer2D::SpriteBatch Renderer2D::CreateSpriteBatch(int spriteCount)
	{
		return SpriteBatch();
	}

	void Renderer2D::RecreateSpriteBatches()
	{

	}

	void Renderer2D::CreateDefaultShader()
	{
		ShaderFactory factory;
		VertexShader& vertex = factory.Vertex();
		vertex.SetBufferLayout(m_Layout);
		ShaderVariablePtr viewMatrix = vertex.RendererUniform(RendererUniform::ViewMatrix);
		ShaderVariablePtr projectionMatrix = vertex.RendererUniform(RendererUniform::ProjectionMatrix);
		ShaderPassVariablePtr outTexCoord = vertex.DeclarePassOut<Vector2f>();
		ShaderPassVariablePtr outColor = vertex.DeclarePassOut<Vector4f>();

		ShaderVariablePtr viewPosition = vertex.DefineVar(ShaderFuncs::Mul(viewMatrix, ShaderFuncs::Vec4(vertex.Stream(POSITION_ATTRIBUTE_INDEX), ShaderLiteral::FromFloat(1.0))));
		vertex.SetVertexPosition(ShaderFuncs::Mul(projectionMatrix, viewPosition));
		vertex.SetVariable(outTexCoord, vertex.Stream(TEXCOORD_ATTRIBUTE_INDEX));
		vertex.SetVariable(outColor, vertex.Stream(COLOR_ATTRIBUTE_INDEX));

		FragmentShader& fragment = factory.Fragment();
		ShaderPassVariablePtr inTexCoord = fragment.DeclarePassIn(outTexCoord);
		ShaderPassVariablePtr inColor = fragment.DeclarePassIn(outColor);

		fragment.SetFragColor(inColor);
		m_DefaultShader = std::make_unique<ShaderLinkContext>(factory.BuildShader());
	}

	void Renderer2D::CreateTextureShader()
	{
		ShaderFactory factory;
		VertexShader& vertex = factory.Vertex();
		vertex.SetBufferLayout(m_Layout);
		ShaderVariablePtr viewMatrix = vertex.RendererUniform(RendererUniform::ViewMatrix);
		ShaderVariablePtr projectionMatrix = vertex.RendererUniform(RendererUniform::ProjectionMatrix);
		ShaderPassVariablePtr outTexCoord = vertex.DeclarePassOut<Vector2f>();
		ShaderPassVariablePtr outColor = vertex.DeclarePassOut<Vector4f>();
		ShaderPassVariablePtr outTextureId = vertex.DeclarePassOut<int>(PassType::Flat);

		ShaderVariablePtr viewPosition = vertex.DefineVar(ShaderFuncs::Mul(viewMatrix, ShaderFuncs::Vec4(vertex.Stream(POSITION_ATTRIBUTE_INDEX), ShaderLiteral::FromFloat(1.0))));
		vertex.SetVertexPosition(ShaderFuncs::Mul(projectionMatrix, viewPosition));
		vertex.SetVariable(outTexCoord, vertex.Stream(TEXCOORD_ATTRIBUTE_INDEX));
		vertex.SetVariable(outColor, vertex.Stream(COLOR_ATTRIBUTE_INDEX));
		vertex.SetVariable(outTextureId, vertex.Stream(TEXTUREID_ATTRIBUTE_INDEX));

		FragmentShader& fragment = factory.Fragment();
		ShaderPassVariablePtr inTexCoord = fragment.DeclarePassIn(outTexCoord);
		ShaderPassVariablePtr inColor = fragment.DeclarePassIn(outColor);
		ShaderPassVariablePtr inTextureId = fragment.DeclarePassIn(outTextureId);
		ShaderVariablePtr textures = fragment.UniformArray<Texture2D>("Textures", (uint32_t)m_TexturesPerDraw);

		ShaderVariablePtr textureColor = fragment.DefineVar(ShaderFuncs::SampleTexture(ShaderFuncs::Index(textures, inTextureId), inTexCoord));
		fragment.SetFragColor(ShaderFuncs::Mul(inColor, textureColor));
		m_TextureShader = std::make_unique<ShaderLinkContext>(factory.BuildShader());
	}

}