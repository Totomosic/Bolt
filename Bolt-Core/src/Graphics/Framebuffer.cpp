#include "bltpch.h"

#include "Framebuffer.h"

namespace Bolt
{

	ClearBuffer operator|(ClearBuffer left, ClearBuffer right)
	{
		return (ClearBuffer)((int)left | (int)right);
	}

	Framebuffer::Framebuffer()
		: m_Id(0), m_Viewport({ 0, 0, 0, 0 }), m_ClearColor(Color::Black)
	{

	}

	Framebuffer::Framebuffer(int width, int height, int samples, bool createOnLoad, const Color& clearColor)
		: m_Id(0), m_Viewport({ 0, 0, width, height }), m_Samples(samples), m_ClearColor(clearColor)
	{
		BLT_ASSERT(samples < GL_MAX_SAMPLES, "Value of samples must be less than " + std::to_string(GL_MAX_SAMPLES));
		Create();
		if (createOnLoad)
		{
			if (!IsMultisampled())
			{
				CreateColorBuffer();
				CreateDepthBuffer();
			}
			else
			{
				CreateColorRenderBuffer();
				CreateDepthRenderBuffer();
			}
		}
	}

	Framebuffer::~Framebuffer()
	{
		if (m_Id != 0)
		{
			GL_CALL(glDeleteFramebuffers(1, &m_Id));
		}
	}

	const Viewport& Framebuffer::GetViewport() const
	{
		return m_Viewport;
	}

	Viewport& Framebuffer::GetViewport()
	{
		return m_Viewport;
	}

	int Framebuffer::Width() const
	{
		return m_Viewport.Width;
	}

	int Framebuffer::Height() const
	{
		return m_Viewport.Height;
	}

	float Framebuffer::Aspect() const
	{
		return (float)Width() / Height();
	}

	id_t Framebuffer::Id() const
	{
		return m_Id;
	}

	int Framebuffer::GetSamples() const
	{
		return m_Samples;
	}

	bool Framebuffer::IsMultisampled() const
	{
		return m_Samples != 0;
	}

	const Color& Framebuffer::ClearColor() const
	{
		return m_ClearColor;
	}

	Color& Framebuffer::ClearColor()
	{
		return m_ClearColor;
	}

	Frustum Framebuffer::ViewFrustum(float nearPlane, float farPlane) const
	{
		return { 0.0f, (float)Width(), 0.0f, (float)Height(), nearPlane, farPlane };
	}

	const Texture2D* Framebuffer::GetTextureBuffer(ColorBuffer buffer) const
	{
		return m_Textures.at(buffer).Get();
	}

	bool Framebuffer::HasTextureBuffer(ColorBuffer buffer) const
	{
		return m_Textures.find(buffer) != m_Textures.end();
	}

	RenderBuffer Framebuffer::GetRenderBuffer(ColorBuffer buffer) const
	{
		return m_RenderBuffers.at(buffer);
	}

	bool Framebuffer::HasRenderBuffer(ColorBuffer buffer) const
	{
		return m_RenderBuffers.find(buffer) != m_RenderBuffers.end();
	}

	const Texture2D* Framebuffer::CreateColorBuffer(ColorBuffer buffer)
	{
		TextureCreateOptions createOptions = { WrapMode::Repeat, MagFilter::Linear, MinFilter::Linear, Mipmaps::Disabled };
		return CreateColorBuffer(AssetHandle<Texture2D>(BLT_NEW Texture2D(Width(), Height(), createOptions), true), buffer);
	}

	const Texture2D* Framebuffer::CreateDepthBuffer()
	{
		TextureCreateOptions createOptions = { WrapMode::Repeat, MagFilter::Linear, MinFilter::Linear, Mipmaps::Disabled };
		return CreateDepthBuffer(AssetHandle<Texture2D>(BLT_NEW Texture2D(Width(), Height(), createOptions), true));
	}

	const Texture2D* Framebuffer::CreateColorBuffer(const AssetHandle<Texture2D>& texture, ColorBuffer buffer)
	{
		CreateColorTexture(texture, buffer);
		m_Textures[buffer] = texture;
		return m_Textures[buffer].Get();
	}

	const Texture2D* Framebuffer::CreateDepthBuffer(const AssetHandle<Texture2D>& texture)
	{
		CreateDepthTexture(texture);
		m_Textures[ColorBuffer::Depth] = texture;
		return m_Textures[ColorBuffer::Depth].Get();
	}

	const Texture2D* Framebuffer::CreateColorBuffer(AssetHandle<Texture2D>&& texture, ColorBuffer buffer)
	{
		CreateColorTexture(texture, buffer);
		m_Textures[buffer] = std::move(texture);
		return m_Textures[buffer].Get();
	}

	const Texture2D* Framebuffer::CreateDepthBuffer(AssetHandle<Texture2D>&& texture)
	{
		CreateDepthTexture(texture);
		m_Textures[ColorBuffer::Depth] = std::move(texture);
		return m_Textures[ColorBuffer::Depth].Get();
	}

	RenderBuffer Framebuffer::CreateColorRenderBuffer(ColorBuffer buffer)
	{
		BLT_ASSERT(!HasRenderBuffer(buffer), "RenderBuffer is already attached at buffer " + std::to_string((int)buffer));
		id_t rBufferID;
		GL_CALL(glGenRenderbuffers(1, &rBufferID));
		RenderBuffer rBuffer = { rBufferID, Width(), Height(), GetSamples(), buffer };
		GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, rBufferID));
		Bind();
		if (IsMultisampled())
		{
			GL_CALL(glRenderbufferStorageMultisample(GL_RENDERBUFFER, rBuffer.Samples, GL_RGBA8, rBuffer.Width, rBuffer.Height));
		}
		else
		{
			GL_CALL(glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, rBuffer.Width, rBuffer.Height));
		}
		GL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, (GLenum)buffer, GL_RENDERBUFFER, rBufferID));
		m_RenderBuffers[buffer] = rBuffer;
		return rBuffer;
	}

	RenderBuffer Framebuffer::CreateDepthRenderBuffer()
	{
		BLT_ASSERT(!HasRenderBuffer(ColorBuffer::Depth), "RenderBuffer is already attached at buffer " + std::to_string((int)ColorBuffer::Depth));
		id_t rBufferID;
		GL_CALL(glGenRenderbuffers(1, &rBufferID));
		RenderBuffer rBuffer = { rBufferID, Width(), Height(), GetSamples(), ColorBuffer::Depth };
		GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, rBufferID));
		Bind();
		if (IsMultisampled())
		{
			GL_CALL(glRenderbufferStorageMultisample(GL_RENDERBUFFER, rBuffer.Samples, GL_DEPTH_COMPONENT, rBuffer.Width, rBuffer.Height));
		}
		else
		{
			GL_CALL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, rBuffer.Width, rBuffer.Height));
		}
		GL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, (GLenum)ColorBuffer::Depth, GL_RENDERBUFFER, rBufferID));
		m_RenderBuffers[ColorBuffer::Depth] = rBuffer;
		return rBuffer;
	}

	void Framebuffer::CopyToFramebuffer(const Framebuffer* frameBuffer, ClearBuffer buffer, Filter filter, ColorBuffer readBuffer, ColorBuffer drawBuffer) const
	{
		BLT_ASSERT(GetSamples() >= frameBuffer->GetSamples(), "The value of Samples must be greater than or equal than the destination Framebuffer when copying between Framebuffers");
		BLT_ASSERT(readBuffer != ColorBuffer::Depth, "Cannot copy depth buffer of Framebuffer");
		BLT_ASSERT(!(IsMultisampled() && !(frameBuffer->Width() == Width() && frameBuffer->Height() == Height())), "Cannot copy multisampled Framebuffers with different dimensions");
		GL_CALL(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer->Id()));
		GL_CALL(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_Id));
		GL_CALL(glReadBuffer((GLenum)readBuffer));
		if (frameBuffer->Id() == 0)
		{
			GL_CALL(glDrawBuffer(GL_FRONT_AND_BACK));
		}
		else
		{
			GL_CALL(glDrawBuffer((GLenum)drawBuffer));
		}
		GL_CALL(glBlitFramebuffer(0, 0, Width(), Height(), 0, 0, frameBuffer->Width(), frameBuffer->Height(), (GLbitfield)buffer, (GLenum)filter));
		GL_CALL(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));
		GL_CALL(glBindFramebuffer(GL_READ_FRAMEBUFFER, 0));
	}

	void Framebuffer::Bind() const
	{
		m_Viewport.Bind();
		GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, m_Id));
	}

	void Framebuffer::Unbind() const
	{
		GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}

	void Framebuffer::Clear(ClearBuffer buffer) const
	{
		Bind();
		GL_CALL(glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a));
		GL_CALL(glClear((GLbitfield)buffer));
	}

	void Framebuffer::SetSize(int width, int height)
	{
		m_Viewport.Width = width;
		m_Viewport.Height = height;
		m_Viewport.Bind();
	}

	void Framebuffer::Create()
	{
		GL_CALL(glGenFramebuffers(1, &m_Id));
	}

	void Framebuffer::CreateColorTexture(const AssetHandle<Texture2D>& texture, ColorBuffer buffer)
	{
		BLT_ASSERT(!HasTextureBuffer(buffer), "RenderBuffer is already attached at buffer " + std::to_string((int)buffer));
		BLT_ASSERT(!IsMultisampled(), "Multisampled Framebuffer cannot have texture attachments");
		Bind();
		texture->Bind();
		GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texture->Width(), texture->Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
		texture->SetMagFilter(MagFilter::Linear);
		texture->SetMinFilter(MinFilter::Linear);
		texture->SetWrapMode(WrapMode::ClampToEdge);
		GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, (GLenum)buffer, GL_TEXTURE_2D, texture->Id(), 0));
		GL_CALL(glDrawBuffer((GLenum)buffer));
	}

	void Framebuffer::CreateDepthTexture(const AssetHandle<Texture2D>& texture)
	{
		BLT_ASSERT(!HasTextureBuffer(ColorBuffer::Depth), "RenderBuffer is already attached at buffer " + std::to_string((int)ColorBuffer::Depth));
		BLT_ASSERT(!IsMultisampled(), "Multisampled Framebuffer cannot have texture attachments");
		Bind();
		texture->Bind();
		GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, texture->Width(), texture->Height(), 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr));
		texture->SetMagFilter(MagFilter::Nearest);
		texture->SetMinFilter(MinFilter::Nearest);
		texture->SetWrapMode(WrapMode::ClampToEdge);
		GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture->Id(), 0));
	}

}