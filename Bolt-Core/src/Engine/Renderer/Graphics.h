#pragma once
#include "GLState.h"
#include "Graphics/Window.h"

namespace Bolt
{

	class BLT_API Graphics
	{
	public:
		static Graphics& Get();

	private:
		Window* m_Window;
		GLState m_State;

	public:
		Graphics(Window* window);

		const Framebuffer* DefaultFramebuffer();
		const GLState& GetState() const;
		GLState& GetState();

		void RenderScene();

		friend class Initializer;
		friend class Destructor;

	private:
		static void Initialize(Window* window);
		static void Terminate();

	};

}