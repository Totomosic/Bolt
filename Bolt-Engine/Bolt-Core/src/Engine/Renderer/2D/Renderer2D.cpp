#include "bltpch.h"
#include "Renderer2D.h"
#include "Graphics/Resources/Meshes/Materials/Shaders/ShaderFactory.h"
#include "Graphics/Resources/Meshes/Materials/Shaders/Components/ShaderFuncs.h"
#include "../Graphics.h"

namespace Bolt
{

	Renderer2D::Renderer2D()
		: m_Layout(), m_TexturesPerDraw(0), m_SpritesPerDraw(10000), m_Batches(), m_TextureBatches(), m_CurrentBatchIndex(0), m_CurrentTextureBatchIndex(0), m_DefaultShader(), m_TextureShader()
	{
		GL_CALL(glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &m_TexturesPerDraw));
		m_Layout.AddAttribute<Vector3f>(POSITION_ATTRIBUTE_INDEX, 1);
		m_Layout.AddAttribute<Vector2f>(TEXCOORD_ATTRIBUTE_INDEX, 1);
		m_Layout.AddAttribute<byte>(COLOR_ATTRIBUTE_INDEX, 4, true);
		m_Layout.AddAttribute<int>(TEXTUREID_ATTRIBUTE_INDEX, 1);

		CreateDefaultShader();
		CreateTextureShader();
		RecreateSpriteBatches();
	}

	int Renderer2D::GetSpritesPerDraw() const
	{
		return m_SpritesPerDraw;
	}

	void Renderer2D::SetSpritesPerDraw(int spriteCount)
	{
		m_SpritesPerDraw = spriteCount;
		RecreateSpriteBatches();
	}

	void Renderer2D::BeginScene(const RenderCamera& camera, const Framebuffer* renderTarget)
	{
		if (renderTarget == nullptr)
		{
			renderTarget = Graphics::Get().DefaultFramebuffer();
		}
		renderTarget->Bind();
		ApplyRendererUniforms(*m_DefaultShader, camera.ViewMatrix, camera.ProjectionMatrix);
		ApplyRendererUniforms(*m_TextureShader, camera.ViewMatrix, camera.ProjectionMatrix);
	}

	void Renderer2D::EndScene()
	{
		m_CurrentBatchIndex = 0;
		m_CurrentTextureBatchIndex = 0;
		for (auto& batch : m_Batches)
		{
			if (batch.VerticesPtr)
			{
				batch.Vertices->GetVertexBuffer(0).Unmap();
			}
			if (batch.IndicesPtr)
			{
				batch.Indices->Unmap();
			}
			batch.VerticesPtr = nullptr;
			batch.IndicesPtr = nullptr;
		}
		for (auto& batch : m_TextureBatches)
		{
			if (batch.VerticesPtr)
			{
				batch.Vertices->GetVertexBuffer(0).Unmap();
			}
			if (batch.IndicesPtr)
			{
				batch.Indices->Unmap();
			}
			batch.VerticesPtr = nullptr;
			batch.IndicesPtr = nullptr;
		}
	}

	void Renderer2D::Flush()
	{
		if (m_Batches.size() > 0)
		{
			m_DefaultShader->GetShaderInstance().GetShader().Bind();
			SpriteBatch* current = &m_Batches[0];
			int index = 0;
			while (current->SpriteCount > 0)
			{
				current->Vertices->Bind();
				current->Indices->Bind();
				GL_CALL(glDrawElements((GLenum)current->Vertices->GetRenderMode(), current->SpriteCount * INDICES_PER_SPRITE, current->Indices->IndexType(), nullptr));
				index++;
				if (index >= m_Batches.size())
				{
					break;
				}
				current = &m_Batches[index];
			}
		}
		if (m_TextureBatches.size() > 0)
		{
			m_TextureShader->GetShaderInstance().GetShader().Bind();
			SpriteBatch* current = &m_TextureBatches[0];
			int index = 0;
			while (current->SpriteCount > 0)
			{
				for (const auto& pair : current->Textures)
				{
					m_TextureShader->Link("Textures", pair.first, pair.second);
				}
				m_TextureShader->ApplyLinks();
				current->Vertices->Bind();
				current->Indices->Bind();
				GL_CALL(glDrawElements((GLenum)current->Vertices->GetRenderMode(), current->SpriteCount * INDICES_PER_SPRITE, current->Indices->IndexType(), nullptr));
				index++;
				if (index >= m_TextureBatches.size())
				{
					break;
				}
				current = &m_TextureBatches[index];
			}
		}
		for (auto& batch : m_Batches)
		{
			batch.SpriteCount = 0;
		}
		for (auto& batch : m_TextureBatches)
		{
			batch.SpriteCount = 0;
		}
	}

	void Renderer2D::DrawSprite(float x0, float y0, float width, float height, const Color& color, float rotation, const ResourcePtr<Texture2D>& texture, const TextureFrame& frame)
	{
		SpriteBatch* currentBatch = nullptr;
		bool isTextured = texture != nullptr;
		int textureUnit = 0;
		if (isTextured)
		{
			currentBatch = &m_TextureBatches[m_CurrentTextureBatchIndex];
			if (currentBatch->Textures.find(texture) != currentBatch->Textures.end())
			{
				textureUnit = currentBatch->Textures[texture];
			}
			else
			{
				textureUnit = currentBatch->Textures.size();
				currentBatch->Textures[texture] = textureUnit;
			}
		}
		else
		{
			currentBatch = &m_Batches[m_CurrentBatchIndex];
		}
		if (!currentBatch->Vertices || !currentBatch->Indices)
		{
			*currentBatch = CreateSpriteBatch(m_SpritesPerDraw, m_Layout);
		}
		if (!currentBatch->VerticesPtr)
		{
			currentBatch->VerticesPtr = currentBatch->Vertices->GetVertexBuffer(0).Map(Access::Write);
		}
		if (!currentBatch->IndicesPtr)
		{
			currentBatch->IndicesPtr = currentBatch->Indices->Map(Access::Write);
		}
		int currentVertex = currentBatch->SpriteCount * VERTICES_PER_SPRITE;
		int vertexOffset = currentVertex;
		int indexOffset = currentBatch->SpriteCount * INDICES_PER_SPRITE;
		SpriteVertex* vertexPtr = ((SpriteVertex*)currentBatch->VerticesPtr) + vertexOffset;
		uint32_t* indexPtr = ((uint32_t*)currentBatch->IndicesPtr) + indexOffset;

		Vector4<byte> c = color.ToBytes();

		Matrix3f matrix = Matrix3f::Rotation(rotation, Vector3f(0, 0, 1));
		float hw = width / 2.0f;
		float hh = width / 2.0f;

		vertexPtr->Position = matrix * Vector3f(-hw, hh, 0) + Vector3f(x0 + hw, y0 + hh, 0);
		vertexPtr->TexCoord = Vector2f(frame.x, frame.y + frame.h);
		vertexPtr->Color = c;
		vertexPtr->TextureUnit = textureUnit;
		vertexPtr++;
		vertexPtr->Position = matrix * Vector3f(-hw, -hh, 0) + Vector3f(x0 + hw, y0 + hh, 0);
		vertexPtr->TexCoord = Vector2f(frame.x, frame.y);
		vertexPtr->Color = c;
		vertexPtr->TextureUnit = textureUnit;
		vertexPtr++;
		vertexPtr->Position = matrix * Vector3f(hw, -hh, 0) + Vector3f(x0 + hw, y0 + hh, 0);
		vertexPtr->TexCoord = Vector2f(frame.x + frame.w, frame.y);
		vertexPtr->Color = c;
		vertexPtr->TextureUnit = textureUnit;
		vertexPtr++;
		vertexPtr->Position = matrix * Vector3f(hw, hh, 0) + Vector3f(x0 + hw, y0 + hh, 0);
		vertexPtr->TexCoord = Vector2f(frame.x + frame.w, frame.y + frame.h);
		vertexPtr->Color = c;
		vertexPtr->TextureUnit = textureUnit;

		*indexPtr++ = currentVertex + 0;
		*indexPtr++ = currentVertex + 1;
		*indexPtr++ = currentVertex + 2;
		*indexPtr++ = currentVertex + 0;
		*indexPtr++ = currentVertex + 2;
		*indexPtr++ = currentVertex + 3;
		currentBatch->SpriteCount++;

		if (currentBatch->SpriteCount >= m_SpritesPerDraw || currentBatch->Textures.size() >= m_TexturesPerDraw)
		{
			if (isTextured)
			{
				m_CurrentTextureBatchIndex++;
				if (m_TextureBatches.size() <= m_CurrentTextureBatchIndex)
				{
					m_TextureBatches.push_back({});
				}
			}
			else
			{
				m_CurrentBatchIndex++;
				if (m_Batches.size() <= m_CurrentBatchIndex)
				{
					m_Batches.push_back({});
				}
			}
		}
	}

	void Renderer2D::DrawRectangle(float x, float y, float width, float height, const Color& color, float rotation)
	{
		DrawSprite(x, y, width, height, color, rotation);
	}

	void Renderer2D::DrawTexture(float x, float y, float width, float height, const ResourcePtr<Texture2D>& texture, float rotation, const TextureFrame& frame)
	{
		DrawSprite(x, y, width, height, Color::White, rotation, texture, frame);
	}

	void Renderer2D::DrawString(const blt::string& string, const ResourcePtr<Font>& font, float x, float y, const Color& color)
	{

	}

	void Renderer2D::DrawString(const blt::string& string, float x, float y, const Color& color)
	{

	}

	void Renderer2D::DrawEllipse(float x, float y, float width, float height, const Color& color, const ResourcePtr<Texture2D>& texture, const TextureFrame& frame)
	{

	}

	void Renderer2D::DrawLine(float x0, float y0, float x1, float y1, float width, const Color& color)
	{

	}

	Renderer2D::SpriteBatch Renderer2D::CreateSpriteBatch(int spriteCount, const BufferLayout& layout)
	{
		BLT_ASSERT(sizeof(SpriteVertex) == layout.Size(), "Invalid layout");
		SpriteBatch batch;
		batch.Vertices = std::make_unique<VertexArray>();
		batch.Indices = std::make_unique<IndexBuffer>(spriteCount * INDICES_PER_SPRITE, BufferUsage::DynamicDraw);
		batch.SpriteCount = 0;
		batch.Vertices->CreateVertexBuffer(spriteCount * VERTICES_PER_SPRITE * layout.Size(), layout, BufferUsage::DynamicDraw);
		return batch;
	}

	void Renderer2D::RecreateSpriteBatches()
	{
		m_Batches.clear();
		m_TextureBatches.clear();
		m_CurrentBatchIndex = 0;
		m_CurrentTextureBatchIndex = 0;
		m_Batches.push_back(CreateSpriteBatch(m_SpritesPerDraw, m_Layout));
		m_TextureBatches.push_back(CreateSpriteBatch(m_SpritesPerDraw, m_Layout));
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

	void Renderer2D::ApplyRendererUniforms(const ShaderLinkContext& shader, const Matrix4f& viewMatrix, const Matrix4f& projectionMatrix) const
	{
		const Shader& s = shader.GetShaderInstance().GetShader();
		s.Bind();
		for (const RendererUniformLocation& uniform : shader.GetShaderInstance().GetRendererUniforms())
		{
			switch (uniform.Uniform)
			{
			case RendererUniform::ViewMatrix:
				s.SetUniform(uniform.Location, viewMatrix);
				break;
			case RendererUniform::ProjectionMatrix:
				s.SetUniform(uniform.Location, projectionMatrix);
				break;
			}
		}
	}

}