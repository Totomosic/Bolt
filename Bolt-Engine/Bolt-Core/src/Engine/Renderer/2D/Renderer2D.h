#pragma once
#include "../RenderCamera.h"
#include "Core/Math/Matrices/Matrix4.h"
#include "Graphics/Buffers/VertexArray.h"
#include "Graphics/Buffers/IndexBuffer.h"
#include "Graphics/Resources/Textures/Texture2D.h"
#include "Graphics/Resources/ResourcePtr.h"
#include "Graphics/Framebuffer.h"

#include "Graphics/Resources/Meshes/Materials/Shaders/ShaderLinkContext.h"
#include "TextureFrame.h"

namespace Bolt
{

	class BLT_API Renderer2D
	{
	public:
		static constexpr int VERTICES_PER_SPRITE = 4;
		static constexpr int INDICES_PER_SPRITE = 6;

		static constexpr int POSITION_ATTRIBUTE_INDEX = 0;
		static constexpr int TEXCOORD_ATTRIBUTE_INDEX = 1;
		static constexpr int COLOR_ATTRIBUTE_INDEX = 2;
		static constexpr int TEXTUREID_ATTRIBUTE_INDEX = 3;

	private:
		struct BLT_API SpriteBatch
		{
		public:
			std::unique_ptr<VertexArray> Vertices;
			std::unique_ptr<IndexBuffer> Indices;
			std::unordered_map<ResourcePtr<Texture2D>, int> Textures;
			int SpriteCount = 0;

			void* VerticesPtr = nullptr;
			void* IndicePtr = nullptr;
		};

	private:
		BufferLayout m_Layout;
		int m_TexturesPerDraw;
		int m_SpritesPerDraw;
		std::vector<SpriteBatch> m_Batches;
		std::vector<SpriteBatch> m_TextureBatches;
		SpriteBatch* m_CurrentBatch;

		std::unique_ptr<ShaderLinkContext> m_DefaultShader;
		std::unique_ptr<ShaderLinkContext> m_TextureShader;

	public:
		Renderer2D();

		int GetSpritesPerDraw() const;

		void SetSpritesPerDraw(int spriteCount);

		void BeginScene(const RenderCamera& camera, const Framebuffer* renderTarget = nullptr);
		void EndScene();
		void Flush();

		void DrawSprite(float x0, float y0, float width, float height, const Color& color = Color::White, const ResourcePtr<Texture2D>& texture = nullptr, const TextureFrame& frame = {});

	private:
		static SpriteBatch CreateSpriteBatch(int spriteCount);
		void RecreateSpriteBatches();
		void CreateDefaultShader();
		void CreateTextureShader();

	};

}