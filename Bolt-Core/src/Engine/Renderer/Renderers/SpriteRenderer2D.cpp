#include "SpriteRenderer2D.h"
#include "..\RenderMethods\DefaultRenderMethod.h"
#include "..\..\Scene\SceneGraph\Query\__Query__.h"

namespace Bolt
{

	SpriteRenderer2D::SpriteRenderer2D() : Renderer(std::make_unique<DefaultRenderMethod>()),
		m_Vao(std::make_unique<VertexArray>()), m_Iao(std::make_unique<IndexArray>()), m_Ibo(nullptr), m_Vbo(nullptr), m_IndexCount(0), m_MappedPtr(nullptr)
	{
		BufferLayout layout;
		layout.AddAttribute<Vector3f>(1);
		layout.AddAttribute<Vector3f>(1);
		layout.AddAttribute<Vector2f>(1);
		layout.AddAttribute<byte>(4, true);
		layout.AddAttribute<float>(1);
		m_Vbo = &m_Vao->CreateVertexBuffer(MAX_VERTICES * layout.Size(), layout, BufferUsage::DynamicDraw);
		m_Ibo = &m_Iao->AddIndexBuffer(std::make_unique<IndexBuffer>(MAX_INDICES, BufferUsage::StaticDraw));

		IndexIterator<> it = m_Iao->Begin();
		uint offset = 0;
		for (uint i = 0; i < MAX_INDICES; i += 6)
		{
			*it = offset + 0;
			it++;
			*it = offset + 1;
			it++;
			*it = offset + 2;
			it++;
			*it = offset + 0;
			it++;
			*it = offset + 2;
			it++;
			*it = offset + 3;
			it++;
			offset += 4;
		}
	}

	void SpriteRenderer2D::Begin(const RenderPass* renderPass) const
	{
		Renderer::Begin(renderPass);
		renderPass->RenderTarget->Clear(ClearBuffer::Depth);
		m_MappedPtr = (SpriteVertex*)m_Vbo->Map(Access::Write);
		m_IndexCount = 0;
		m_Textures.clear();
		m_TextureLookup.clear();
	}

	void SpriteRenderer2D::Render(const RenderPass* renderPass) const
	{
		if (&SceneManager::CurrentScene() == nullptr)
		{
			return;
		}
		std::vector<const Layer*> layers = (renderPass->LayerMask == RenderPass::ALL_LAYERS) ? SceneManager::CurrentScene().GetAllLayers() : SceneManager::CurrentScene().GetLayers(renderPass->LayerMask);
		for (const Layer* layer : layers)
		{
			std::vector<GameObject*> defaultObjects = layer->Graph().Query(SGQComponents({ typeid(MeshRenderer) })).GameObjects;
			std::sort(defaultObjects.begin(), defaultObjects.end(), [](GameObject* left, GameObject* right)
			{
				return left->transform().Position().y > right->transform().Position().y;
			});

			Camera* camera = layer->ActiveCamera();

			bool hasCameraOverride = renderPass->CameraOverrides.find(layer->Id()) != renderPass->CameraOverrides.end();
			Matrix4f viewMatrix = (hasCameraOverride) ? renderPass->CameraOverrides.at(layer->Id()).ViewMatrix : layer->ActiveCamera()->ViewMatrix();
			Matrix4f projectionMatrix = (hasCameraOverride) ? renderPass->CameraOverrides.at(layer->Id()).ProjectionMatrix : layer->ActiveCamera()->ProjectionMatrix();

			RenderGameObjects(defaultObjects, viewMatrix, projectionMatrix, renderPass->Uniforms);
		}
	}

	void SpriteRenderer2D::End(const RenderPass* renderPass) const
	{
		Renderer::End(renderPass);
	}

	void SpriteRenderer2D::RenderGameObjects(const std::vector<GameObject*>& objects, const Matrix4f& viewMatrix, const Matrix4f& projectionMatrix, const UniformManager& uniforms) const
	{
		for (GameObject* object : objects)
		{
			const Mesh& mesh = object->Components().GetComponent<MeshRenderer>().Mesh;
			for (int i = 0; i < mesh.Models.size(); i++)
			{
				const Mesh::ModelGroup& model = mesh.Models[i];
				BLT_ASSERT(model.MaterialIndices.size() >= model.Model->Data().Indices.IndexBufferCount(), "Size of MaterialIndices should be equal to or greater than the number of Index Buffers Attached");
				for (int j = 0; j < model.MaterialIndices.size(); j++)
				{
					const Material* material = &mesh.Materials[model.MaterialIndices[j]];
					RenderBatch batch;
					uniforms.UploadAll(material->Shader.Get());
					batch.Material = material;
					batch.Geometry.push_back({ model.Model->Data().Vertices.get(), model.Model->Data().Indices.GetIndexBuffer(j).get(), object->transform().TransformMatrix() * model.Transform });
					Submit(batch);
				}
			}
		}	
		m_MappedPtr = nullptr;
		m_Vbo->Unmap();
		Flush(viewMatrix, projectionMatrix);
	}

	void SpriteRenderer2D::Submit(const RenderBatch& renderBatch) const
	{
		Vector3f tl = renderBatch.Geometry[0].Transform * Vector3f(-0.5f, 0.5f, 0);
		Vector3f bl = renderBatch.Geometry[0].Transform * Vector3f(-0.5f, -0.5f, 0);
		Vector3f br = renderBatch.Geometry[0].Transform * Vector3f(0.5f, -0.5f, 0);
		Vector3f tr = renderBatch.Geometry[0].Transform * Vector3f(0.5f, 0.5f, 0);
		Vector3f normal = Vector3f(0, 0, 1);
		Vector2f tlT = Vector2f(0, 1);
		Vector2f blT = Vector2f(0, 0);
		Vector2f brT = Vector2f(1, 0);
		Vector2f trT = Vector2f(1, 1);
		Color c = renderBatch.Material->BaseColor;
		Vector4<byte> color = c.ToBytes();

		int tid = 0;
		if (renderBatch.Material->Textures.Textures.size() > 0)
		{
			const Texture* texture = renderBatch.Material->Textures.Textures[0].Get();
			if (m_TextureLookup.find(texture) != m_TextureLookup.end())
			{
				tid = m_TextureLookup.at(texture);
			}
			else
			{
				int index = m_Textures.size() + 1;
				m_Textures.push_back(texture);
				m_TextureLookup[texture] = index;
				tid = index;
			}
		}

		m_MappedPtr->Position = tl;
		m_MappedPtr->Normal = normal;
		m_MappedPtr->TexCoord = tlT;
		m_MappedPtr->Color = color;
		m_MappedPtr->TID = tid;
		m_MappedPtr++;

		m_MappedPtr->Position = bl;
		m_MappedPtr->Normal = normal;
		m_MappedPtr->TexCoord = blT;
		m_MappedPtr->Color = color;
		m_MappedPtr->TID = tid;
		m_MappedPtr++;

		m_MappedPtr->Position = br;
		m_MappedPtr->Normal = normal;
		m_MappedPtr->TexCoord = brT;
		m_MappedPtr->Color = color;
		m_MappedPtr->TID = tid;
		m_MappedPtr++;

		m_MappedPtr->Position = tr;
		m_MappedPtr->Normal = normal;
		m_MappedPtr->TexCoord = trT;
		m_MappedPtr->Color = color;
		m_MappedPtr->TID = tid;
		m_MappedPtr++;

		m_IndexCount += 6;
	}

	void SpriteRenderer2D::Flush(const Matrix4f& viewMatrix, const Matrix4f& projMatrix) const
	{
		Material mat;
		mat.Shader = Shader::SpriteTexture();
		mat.Textures.Textures = m_Textures;
		mat.RenderOptions.DepthFunc = DepthFunction::Lequal;
		RenderBatch renderable;
		renderable.Material = &mat;
		renderable.Geometry.push_back({ m_Vao.get(), m_Ibo, Matrix4f::Identity(), m_IndexCount });
		(*m_Method)(renderable, viewMatrix, projMatrix);
	}

}