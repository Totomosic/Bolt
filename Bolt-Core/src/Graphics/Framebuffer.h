#pragma once
#include "Bolt-Core.h"
#include "GLprivate.h"
#include "Structs\Viewport.h"

#include "Resources\Textures\Texture2D.h"
#include "RenderBuffer.h"

#include "Resources\ResourcePtr.h"

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
	
	class BLT_API Framebuffer : public GLprivate
	{
	protected:
		id_t m_Id;
		Viewport m_Viewport;
		Color m_ClearColor;
		int m_Samples;

		std::unordered_map<ColorBuffer, ResourcePtr<Texture2D>> m_Textures;
		std::unordered_map<ColorBuffer, RenderBuffer> m_RenderBuffers;

	private:
		Framebuffer(); // Default window Framebuffer

	public:
		Framebuffer(int width, int height, int samples = 0, bool createOnLoad = true, const Color& clearColor = Color::Black);
		~Framebuffer() override;

		const Viewport& GetViewport() const;
		Viewport& GetViewport();
		int Width() const;
		int Height() const;
		float Aspect() const;
		id_t ID() const;
		int Samples() const;
		bool IsMultisampled() const;
		const Color& ClearColor() const;
		Color& ClearColor();
		Frustum ViewFrustum(float nearPlane = 1.0f, float farPlane = 1000.0f) const;

		const Texture2D* GetTextureBuffer(ColorBuffer buffer) const;
		bool HasTextureBuffer(ColorBuffer buffer) const;
		RenderBuffer GetRenderBuffer(ColorBuffer buffer) const;
		bool HasRenderBuffer(ColorBuffer buffer) const;
		virtual const Texture2D* CreateColorBuffer(ColorBuffer buffer = ColorBuffer::Color0);
		virtual const Texture2D* CreateDepthBuffer();
		virtual const Texture2D* CreateColorBuffer(const ResourcePtr<Texture2D>& texture, ColorBuffer buffer = ColorBuffer::Color0);
		virtual const Texture2D* CreateDepthBuffer(const ResourcePtr<Texture2D>&  texture);
		virtual RenderBuffer CreateColorRenderBuffer(ColorBuffer buffer = ColorBuffer::Color0);
		virtual RenderBuffer CreateDepthRenderBuffer();

		void CopyToFramebuffer(const Framebuffer* frameBuffer, ClearBuffer buffer = ClearBuffer::Color | ClearBuffer::Depth, Filter filter = Filter::Nearest, ColorBuffer readBuffer = ColorBuffer::Color0, ColorBuffer drawBuffer = ColorBuffer::Color0) const;

		void Bind() const;
		void Unbind() const;
		void Clear(ClearBuffer buffer = ClearBuffer::Color | ClearBuffer::Depth) const;

		friend class Window;

	private:
		void Create();

	protected:
		void CreateColorTexture(const ResourcePtr<Texture2D>&  texture, ColorBuffer buffer = ColorBuffer::Color0);
		void CreateDepthTexture(const ResourcePtr<Texture2D>&  texture);

	};

	typedef Framebuffer RenderTarget;

}