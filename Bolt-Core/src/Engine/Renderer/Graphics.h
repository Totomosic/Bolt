#pragma once
#include "Bolt-Graphics.h"
#include "GLState.h"

namespace Bolt
{

	class BLT_API Graphics
	{
	private:
		static std::unique_ptr<Graphics> s_Instance;

	public:
		static Graphics& Get();

	private:
		Window* m_Window;
		GLState m_State;

	public:
		Graphics();

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