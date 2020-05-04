#pragma once
#include "BoltLib/Math/Matrices/Matrix4.h"
#include "Graphics/Buffers/VertexArray.h"
#include "Graphics/Buffers/IndexBuffer.h"
#include "Graphics/Assets/Textures/Texture2D.h"
#include "Graphics/Assets/AssetHandle.h"
#include "Graphics/Framebuffer.h"
#include "../RenderingContext.h"

#include "Graphics/Assets/Meshes/Materials/Shaders/ShaderLinkContext.h"
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
			std::unique_ptr<VertexArray> Vertices = nullptr;
			std::unique_ptr<IndexBuffer> Indices = nullptr;
			std::unordered_map<AssetHandle<Texture2D>, int> Textures;
			int SpriteCount = 0;

			void* VerticesPtr = nullptr;
			void* IndicesPtr = nullptr;
		};

#pragma pack(push, 1)
		struct BLT_API SpriteVertex
		{
		public:
			Vector3f Position;
			Vector2f TexCoord;
			Vector4<byte> Color;
			int TextureUnit;
		};
#pragma pack(pop)

	private:
		BufferLayout m_Layout;
		int m_TexturesPerDraw;
		int m_SpritesPerDraw;
		std::vector<SpriteBatch> m_Batches;
		std::vector<SpriteBatch> m_TextureBatches;
		int m_CurrentBatchIndex;
		int m_CurrentTextureBatchIndex;

		std::unique_ptr<ShaderLinkContext> m_DefaultShader;
		std::unique_ptr<ShaderLinkContext> m_TextureShader;

	public:
		Renderer2D();

		int GetSpritesPerDraw() const;

		void SetSpritesPerDraw(int spriteCount);

		void BeginScene(const RenderCamera& camera, const Framebuffer* renderTarget = nullptr);
		void EndScene();
		void Flush();

		void DrawSprite(float x, float y, float width, float height, const Color& color = Color::White, float rotation = 0, const AssetHandle<Texture2D>& texture = nullptr, const TextureFrame& frame = {});
		void DrawRectangle(float x, float y, float width, float height, const Color& color = Color::White, float rotation = 0);
		void DrawTexture(float x, float y, float width, float height, const AssetHandle<Texture2D>& texture, float rotation = 0, const TextureFrame& frame = {});
		void DrawString(const std::string& string, const AssetHandle<Font>& font, float x, float y, const Color& color = Color::White);
		// Draws string with the default font
		void DrawString(const std::string& string, float x, float y, const Color& color = Color::White);
		void DrawEllipse(float x, float y, float width, float height, const Color& color = Color::White, const AssetHandle<Texture2D>& texture = nullptr, const TextureFrame& frame = {});
		void DrawLine(float x0, float y0, float x1, float y1, float width = 1, const Color & color = Color::White);

	private:
		static SpriteBatch CreateSpriteBatch(int spriteCount, const BufferLayout& layout);
		void RecreateSpriteBatches();
		void CreateDefaultShader();
		void CreateTextureShader();

		void ApplyRendererUniforms(const ShaderLinkContext& shader, const Matrix4f& viewMatrix, const Matrix4f& projectionMatrix) const;

	};

}