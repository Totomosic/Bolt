#include "Types.h"

#include "Graphics.h"
#include "SceneRenderer.h"
#include "GLState.h"

namespace Bolt
{

	Window* Graphics::s_Window = nullptr;

	ResourcePtr<Model> Graphics::s_Rectangle = nullptr;
	ResourcePtr<Model> Graphics::s_Line = nullptr;

	const Framebuffer* Graphics::DefaultFramebuffer()
	{
		return &s_Window->GetFramebuffer();
	}

	void Graphics::Rectangle(float x, float y, float z, float w, float h, const Color& color, const Quaternion& orientation, id_t layerId)
	{
		GameObject* object = SceneManager::CurrentScene().GetLayer(layerId).AddTemporaryGameObject(GameObject());
		object->transform().SetLocalPosition(x, y, z);
		object->transform().SetLocalScale(w, h, 1);
		object->transform().SetLocalOrientation(orientation);

		Mesh mesh;
		mesh.Models.push_back({ s_Rectangle, Matrix4f::Identity(), { 0 } });
		mesh.Materials[0].BaseColor = color;
		object->Components().AddComponent(std::make_unique<MeshRenderer>(mesh));
	}

	void Graphics::Line(float x0, float y0, float z0, float x1, float y1, float z1, const Color& color, id_t layerId)
	{
		Vector3f p0 = Vector3f(x0, y0, z0);
		Vector3f p1 = Vector3f(x1, y1, z1);
		Line(p0, p1, color, layerId);
	}

	void Graphics::Line(const Vector3f& p0, const Vector3f& p1, const Color& color, id_t layerId)
	{
		float distance = Vector3f::Distance(p0, p1);
		if (distance == 0.0f)
		{
			return;
		}
		GameObject* object = SceneManager::CurrentScene().GetLayer(layerId).AddTemporaryGameObject(GameObject());
		object->transform().SetLocalPosition((p0 + p1) / 2.0f);
		object->transform().SetLocalScale(distance, 1, 1);

		Vector3f direction = p1 - p0;
		direction = direction.Normalize();
		Vector3f normal = Vector3f::Cross(Vector3f::Right(), direction);
		float dot = direction.Dot(Vector3f::Right());
		Quaternion orientation = Quaternion::FromAngleAxis(acos(dot), normal);
		object->transform().SetLocalOrientation(orientation);

		Mesh mesh;
		mesh.Models.push_back({ s_Line, Matrix4f::Identity(),{ 0 } });
		mesh.Materials[0].BaseColor = color;
		object->Components().AddComponent(std::make_unique<MeshRenderer>(mesh));
	}

	void Graphics::Text(const blt::string& text, const ResourcePtr<const Font>& font, float x, float y, float z, const Color& color, AlignH horizontalAlign, AlignV verticalAlign, id_t layerId)
	{
		GameObject* object = SceneManager::CurrentScene().GetLayer(layerId).AddTemporaryGameObject(GameObject());
		object->transform().SetLocalPosition(x, y, z);

		Mesh mesh;
		mesh.Models.push_back({ ResourcePtr<Model>(new Model(TextFactory(text, font, Color::White, horizontalAlign, verticalAlign)), true), Matrix4f::Identity(), { 0 } });
		mesh.Materials[0].BaseColor = color;
		mesh.Materials[0].Textures.Textures.push_back(font);
		mesh.Materials[0].Shader = Shader::DefaultFont();
		mesh.Materials[0].RenderOptions.DepthFunc = DepthFunction::Lequal;
		object->Components().AddComponent(std::make_unique<MeshRenderer>(mesh));
	}

	void Graphics::Image(float x, float y, float z, float w, float h, const ResourcePtr<const Texture2D>& texture, const Quaternion& orientation, id_t layerId)
	{
		GameObject* object = SceneManager::CurrentScene().GetLayer(layerId).AddTemporaryGameObject(GameObject());
		object->transform().SetLocalPosition(x, y, z);

		Mesh mesh;
		mesh.Models.push_back({ s_Rectangle, Matrix4f::Scale(w, h, 1), { 0 } });
		mesh.Materials[0].BaseColor = Color::White;
		mesh.Materials[0].Textures.Textures.push_back(texture);
		mesh.Materials[0].Shader = Shader::DefaultTexture();
		object->Components().AddComponent(std::make_unique<MeshRenderer>(mesh));
	}

	void Graphics::RenderScene()
	{
		if (&SceneManager::CurrentScene() == nullptr)
		{
			BLT_CORE_WARN("No Active Scene");
			return;
		}
		DefaultFramebuffer()->Clear();
		SceneRenderer::Render(SceneManager::CurrentScene());
	}

	void Graphics::Initialize(Window* window)
	{
		s_Window = window;
		s_Rectangle = ResourcePtr<Model>(new Model(RectangleFactory(1, 1)), false);
		s_Line = ResourcePtr<Model>(new Model(LineFactory(Vector3f::Right(), 1)), false);
		GLState::Reset();
	}

	void Graphics::Terminate()
	{
		s_Window = nullptr;
		delete s_Rectangle.Release();
		delete s_Line.Release();
	}

}