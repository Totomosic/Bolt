#pragma once
#include "GLState.h"
#include "Graphics/Window.h"
#include "Renderer.h"

namespace Bolt
{

	class BLT_API Graphics
	{
	public:
		static Graphics& Get();

	private:
		Window* m_Window;
		GLState m_State;
		Renderer m_Renderer;

	public:
		Graphics(Window* window);

		const Framebuffer* DefaultFramebuffer();
		const GLState& GetState() const;
		GLState& GetState();
		const Renderer& GetRenderer() const;
		Renderer& GetRenderer();

		void RenderScene();

	};

}