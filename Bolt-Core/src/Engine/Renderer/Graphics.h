#pragma once
#include "Bolt-Graphics.h"
#include "RenderSchedule.h"
#include "Renderer.h"
#include "..\Scene\__Scene__.h"

namespace Bolt
{

	class BLT_API Graphics
	{
	private:
		static Model* s_Rectangle;
		static Model* s_Line;

	private:
		static Window* s_Window;
		static RenderSchedule s_Schedule;
		static std::vector<std::unique_ptr<Renderer>> s_Renderers;

	public:
		Graphics() = delete;

		static const Framebuffer* DefaultFramebuffer();

		static void Rectangle(float x, float y, float z, float w, float h, const Color& color = Color::White, const Quaternion& orientation = Quaternion::Identity(), id_t layerId = 0);
		static void Line(float x0, float y0, float z0, float x1, float y1, float z1, const Color& color = Color::White, id_t layerId = 0);
		static void Line(const Vector3f& p0, const Vector3f& p1, const Color& color = Color::White, id_t layerId = 0);
		static void Text(const blt::string& text, const ResourcePtr<const Font>& font, float x, float y, float z, const Color& color = Color::White, AlignH horizontalAlign = AlignH::Center, AlignV verticalAlign = AlignV::Center, id_t layerId = 0);
		static void Image(float x, float y, float z, float w, float h, const ResourcePtr<const Texture2D>& texture, const Quaternion& orientation = Quaternion::Identity(), id_t layerId = 0);

		static RenderSchedule& Schedule();
		static id_t AddRenderer(std::unique_ptr<Renderer>&& renderer);
		static const Renderer* GetRenderer(id_t id);
		static void RenderScene();

		friend class Initializer;

	private:
		static void Initialize(Window* window);

	};

}