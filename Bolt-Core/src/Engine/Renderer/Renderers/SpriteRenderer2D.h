#pragma once
#include "..\Renderer.h"

namespace Bolt
{

	struct BLT_API SpriteVertex
	{
	public:
		Vector3f Position;
		Vector3f Normal;
		Vector2f TexCoord;
		Vector4<byte> Color;
		float TID;
	};

	// Renderer for rendering 2D Rectangle Sprites
	class BLT_API SpriteRenderer2D : public Renderer
	{
	public:
		static constexpr uint MAX_SPRITES = GAMEOBJECTS_PER_LAYER;
		static constexpr uint MAX_VERTICES = MAX_SPRITES * 4;
		static constexpr uint MAX_INDICES = MAX_SPRITES * 6;

	private:
		std::unique_ptr<VertexArray> m_Vao;
		std::unique_ptr<IndexArray> m_Iao;
		IndexBuffer* m_Ibo;
		VertexBuffer* m_Vbo;
		mutable uint m_IndexCount;
		mutable SpriteVertex* m_MappedPtr;
		mutable std::vector<ResourcePtr<const Texture>> m_Textures;
		mutable std::unordered_map<const Texture*, int> m_TextureLookup;

	public:
		SpriteRenderer2D();

		void Begin(const RenderPass* renderPass) const override;
		void Render(const RenderPass* renderPass) const override;
		void End(const RenderPass* renderPass) const override;

	private:
		void RenderGameObjects(const std::vector<GameObject*>& objects, const Matrix4f& viewMatrix, const Matrix4f& projMatrix, const UniformManager& uniforms) const;
		void Submit(const RenderBatch& renderBatch) const;
		void Flush(const Matrix4f& viewMatrix, const Matrix4f& projMatrix) const;

	};

}