#pragma once
#include "BoltLib/Color.h"
#include "Structs/Viewport.h"
#include "Assets/Textures/Texture2D.h"
#include "RenderBuffer.h"
#include "Assets/AssetHandle.h"

namespace Bolt
{

	BLT_API enum class ClearBuffer : GLenum
	{
		Color = GL_COLOR_BUFFER_BIT,
		Depth = GL_DEPTH_BUFFER_BIT,
		Stencil = GL_STENCIL_BUFFER_BIT
	};

	ClearBuffer operator|(ClearBuffer left, ClearBuffer right);

	BLT_API enum class Filter : GLenum
	{
		Linear = GL_LINEAR,
		Nearest = GL_NEAREST
	};
	
	class BLT_API Framebuffer
	{
	protected:
		id_t m_Id;
		Viewport m_Viewport;
		Color m_ClearColor;
		int m_Samples;

		std::unordered_map<ColorBuffer, AssetHandle<Texture2D>> m_Textures;
		std::unordered_map<ColorBuffer, RenderBuffer> m_RenderBuffers;

	private:
		Framebuffer(); // Default window Framebuffer

	public:
		Framebuffer(int width, int height, int samples = 0, bool createOnLoad = true, const Color& clearColor = Color::Black);
		~Framebuffer();

		const Viewport& GetViewport() const;
		Viewport& GetViewport();
		int Width() const;
		int Height() const;
		float Aspect() const;
		id_t Id() const;
		int GetSamples() const;
		bool IsMultisampled() const;
		const Color& ClearColor() const;
		Color& ClearColor();
		Frustum ViewFrustum(float nearPlane = 1.0f, float farPlane = 1000.0f) const;

		const Texture2D* GetTextureBuffer(ColorBuffer buffer) const;
		bool HasTextureBuffer(ColorBuffer buffer) const;
		RenderBuffer GetRenderBuffer(ColorBuffer buffer) const;
		bool HasRenderBuffer(ColorBuffer buffer) const;
		const Texture2D* CreateColorBuffer(ColorBuffer buffer = ColorBuffer::Color0);
		const Texture2D* CreateDepthBuffer();
		const Texture2D* CreateColorBuffer(const AssetHandle<Texture2D>& texture, ColorBuffer buffer = ColorBuffer::Color0);
		const Texture2D* CreateDepthBuffer(const AssetHandle<Texture2D>& texture);
		const Texture2D* CreateColorBuffer(AssetHandle<Texture2D>&& texture, ColorBuffer buffer = ColorBuffer::Color0);
		const Texture2D* CreateDepthBuffer(AssetHandle<Texture2D>&& texture);
		RenderBuffer CreateColorRenderBuffer(ColorBuffer buffer = ColorBuffer::Color0);
		RenderBuffer CreateDepthRenderBuffer();

		void CopyToFramebuffer(const Framebuffer* frameBuffer, ClearBuffer buffer = ClearBuffer::Color | ClearBuffer::Depth, Filter filter = Filter::Nearest, 
			ColorBuffer readBuffer = ColorBuffer::Color0, ColorBuffer drawBuffer = ColorBuffer::Color0) const;

		void Bind() const;
		void Unbind() const;
		void Clear(ClearBuffer buffer = ClearBuffer::Color | ClearBuffer::Depth) const;
		void SetSize(int width, int height);

		friend class Window;
		friend struct WindowData;

	private:
		void Create();

	protected:
		void CreateColorTexture(const AssetHandle<Texture2D>&  texture, ColorBuffer buffer = ColorBuffer::Color0);
		void CreateDepthTexture(const AssetHandle<Texture2D>&  texture);

	};

	typedef Framebuffer RenderTarget;

}